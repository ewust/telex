#ifndef _IP_SET_CHASH_H
#define _IP_SET_CHASH_H

#include <linux/netfilter/ipset/jhash.h>
#include <linux/netfilter/ipset/slist.h>
#include <linux/netfilter/ipset/ip_set_timeout.h>

/* Cacheline friendly hash with resizing when linear searching becomes too
 * long. Internally jhash is used with the assumption that the size of the
 * stored data is a multiple of sizeof(u32). If storage supports timeout,
 * the timeout field must be the last one in the data structure - that field
 * is ignored when computing the hash key.
 */

/* Number of elements to store in an array block */
#define CHASH_DEFAULT_ARRAY_SIZE        4
/* Number of arrays: max ARRAY_SIZE * CHAIN_LIMIT "long" chains */
#define CHASH_DEFAULT_CHAIN_LIMIT       3

/* Book-keeping of the prefixes added to the set */
struct chash_nets {
	u8 cidr;		/* the different cidr values in the set */
	u32 nets;		/* number of elements per cidr */
};

struct htable {
	u8 htable_bits;		/* size of hash table == 2^htable_bits */
	struct slist htable[0];	/* hashtable of single linked lists */
};

struct chash {
	struct htable *table;	/* the hash table */
	u32 maxelem;		/* max elements in the hash */
	u32 elements;		/* current element (vs timeout) */
	u32 initval;		/* random jhash init value */
	u32 timeout;		/* timeout value, if enabled */
	struct timer_list gc;	/* garbage collection when timeout enabled */
	u8 array_size;		/* number of elements in an array */
	u8 chain_limit;		/* max number of arrays */
#ifdef IP_SET_HASH_WITH_NETMASK
	u8 netmask;		/* netmask value for subnets to store */
#endif
#ifdef IP_SET_HASH_WITH_NETS
	struct chash_nets nets[0]; /* book-keeping of prefixes */
#endif
};

/* Compute htable_bits from the user input parameter hashsize */
static inline u8
htable_bits(u32 hashsize)
{
	/* Assume that hashsize == 2^htable_bits */
	u8 bits = fls(hashsize - 1);
	if (jhash_size(bits) != hashsize)
		/* Round up to the first 2^n value */
		bits = fls(hashsize);

	return bits;
}

#ifdef IP_SET_HASH_WITH_NETS

#define SET_HOST_MASK(family)	(family == AF_INET ? 32 : 128)

/* Network cidr size book keeping when the hash stores different
 * sized networks */
static inline void
add_cidr(struct chash *h, u8 cidr, u8 host_mask)
{
	u8 i;

	++h->nets[cidr-1].nets;

	pr_debug("add_cidr added %u: %u", cidr, h->nets[cidr-1].nets);

	if (h->nets[cidr-1].nets > 1)
		return;

	/* New cidr size */
	for (i = 0; i < host_mask && h->nets[i].cidr; i++) {
		/* Add in increasing prefix order, so larger cidr first */
		if (h->nets[i].cidr < cidr)
			swap(h->nets[i].cidr, cidr);
	}
	if (i < host_mask)
		h->nets[i].cidr = cidr;
}

static inline void
del_cidr(struct chash *h, u8 cidr, u8 host_mask)
{
	u8 i;

	--h->nets[cidr-1].nets;

	pr_debug("del_cidr deleted %u: %u", cidr, h->nets[cidr-1].nets);

	if (h->nets[cidr-1].nets != 0)
		return;

	/* All entries with this cidr size deleted, so cleanup h->cidr[] */
	for (i = 0; i < host_mask - 1 && h->nets[i].cidr; i++) {
		if (h->nets[i].cidr == cidr)
			h->nets[i].cidr = cidr = h->nets[i+1].cidr;
	}
	h->nets[i - 1].cidr = 0;
}
#endif

/* Destroy the hashtable part of the set */
static void
chash_destroy(struct htable *ht)
{
	struct slist *n, *tmp;
	u32 i;

	for (i = 0; i < jhash_size(ht->htable_bits); i++)
		slist_for_each_safe(n, tmp, &ht->htable[i])
			/* FIXME: use slab cache */
			kfree(n);

	ip_set_free(ht);
}

/* Calculate the actual memory size of the set data */
static size_t
chash_memsize(const struct chash *h, size_t dsize, u8 host_mask)
{
	struct slist *n;
	u32 i;
	struct htable *ht = h->table;
	size_t memsize = sizeof(*h)
#ifdef IP_SET_HASH_WITH_NETS
			 + sizeof(struct chash_nets) * host_mask
#endif
			 + jhash_size(ht->htable_bits) * sizeof(struct slist);

	for (i = 0; i < jhash_size(ht->htable_bits); i++)
		slist_for_each(n, &ht->htable[i])
			memsize += sizeof(struct slist)
				+ h->array_size * dsize;

	return memsize;
}

/* Flush a hash type of set: destroy all elements */
static void
ip_set_hash_flush(struct ip_set *set)
{
	struct chash *h = set->data;
	struct htable *ht = h->table;
	struct slist *n, *tmp;
	u32 i;

	for (i = 0; i < jhash_size(ht->htable_bits); i++) {
		slist_for_each_safe(n, tmp, &ht->htable[i])
			/* FIXME: slab cache */
			kfree(n);
		ht->htable[i].next = NULL;
	}
#ifdef IP_SET_HASH_WITH_NETS
	memset(h->nets, 0, sizeof(struct chash_nets)
			   * SET_HOST_MASK(set->family));
#endif
	h->elements = 0;
}

/* Destroy a hash type of set */
static void
ip_set_hash_destroy(struct ip_set *set)
{
	struct chash *h = set->data;

	if (with_timeout(h->timeout))
		del_timer_sync(&h->gc);

	chash_destroy(h->table);
	kfree(h);

	set->data = NULL;
}

#define JHASH2(data, initval, htable_bits)				 \
(jhash2((u32 *)(data), sizeof(struct type_pf_elem)/sizeof(u32), initval) \
	& jhash_mask(htable_bits))

#endif /* _IP_SET_CHASH_H */

#define CONCAT(a, b, c)		a##b##c
#define TOKEN(a, b, c)		CONCAT(a, b, c)

/* Type/family dependent function prototypes */

#define type_pf_data_equal	TOKEN(TYPE, PF, _data_equal)
#define type_pf_data_isnull	TOKEN(TYPE, PF, _data_isnull)
#define type_pf_data_copy	TOKEN(TYPE, PF, _data_copy)
#define type_pf_data_swap	TOKEN(TYPE, PF, _data_swap)
#define type_pf_data_zero_out	TOKEN(TYPE, PF, _data_zero_out)
#define type_pf_data_netmask	TOKEN(TYPE, PF, _data_netmask)
#define type_pf_data_list	TOKEN(TYPE, PF, _data_list)
#define type_pf_data_tlist	TOKEN(TYPE, PF, _data_tlist)

#define type_pf_elem		TOKEN(TYPE, PF, _elem)
#define type_pf_telem		TOKEN(TYPE, PF, _telem)
#define type_pf_data_timeout	TOKEN(TYPE, PF, _data_timeout)
#define type_pf_data_expired	TOKEN(TYPE, PF, _data_expired)
#define type_pf_data_swap_timeout TOKEN(TYPE, PF, _data_swap_timeout)
#define type_pf_data_timeout_set TOKEN(TYPE, PF, _data_timeout_set)

#define type_pf_chash_readd	TOKEN(TYPE, PF, _chash_readd)
#define type_pf_chash_del_elem	TOKEN(TYPE, PF, _chash_del_elem)
#define type_pf_chash_add	TOKEN(TYPE, PF, _chash_add)
#define type_pf_chash_del	TOKEN(TYPE, PF, _chash_del)
#define type_pf_chash_test_cidrs TOKEN(TYPE, PF, _chash_test_cidrs)
#define type_pf_chash_test	TOKEN(TYPE, PF, _chash_test)

#define type_pf_chash_treadd	TOKEN(TYPE, PF, _chash_treadd)
#define type_pf_chash_del_telem	TOKEN(TYPE, PF, _chash_del_telem)
#define type_pf_chash_expire	TOKEN(TYPE, PF, _chash_expire)
#define type_pf_chash_tadd	TOKEN(TYPE, PF, _chash_tadd)
#define type_pf_chash_tdel	TOKEN(TYPE, PF, _chash_tdel)
#define type_pf_chash_ttest_cidrs TOKEN(TYPE, PF, _chash_ttest_cidrs)
#define type_pf_chash_ttest	TOKEN(TYPE, PF, _chash_ttest)

#define type_pf_resize		TOKEN(TYPE, PF, _resize)
#define type_pf_tresize		TOKEN(TYPE, PF, _tresize)
#define type_pf_flush		ip_set_hash_flush
#define type_pf_destroy		ip_set_hash_destroy
#define type_pf_head		TOKEN(TYPE, PF, _head)
#define type_pf_list		TOKEN(TYPE, PF, _list)
#define type_pf_tlist		TOKEN(TYPE, PF, _tlist)
#define type_pf_same_set	TOKEN(TYPE, PF, _same_set)
#define type_pf_kadt		TOKEN(TYPE, PF, _kadt)
#define type_pf_uadt		TOKEN(TYPE, PF, _uadt)
#define type_pf_gc		TOKEN(TYPE, PF, _gc)
#define type_pf_gc_init		TOKEN(TYPE, PF, _gc_init)
#define type_pf_variant		TOKEN(TYPE, PF, _variant)
#define type_pf_tvariant	TOKEN(TYPE, PF, _tvariant)

/* Flavour without timeout */

/* Get the ith element from the array block n */
#define chash_data(n, i)					\
(struct type_pf_elem *)((char *)(n) + sizeof(struct slist)	\
			+ (i)*sizeof(struct type_pf_elem))

/* Add an element to the hash table when resizing the set:
 * we spare the maintenance of the internal counters. */
static int
type_pf_chash_readd(struct chash *h, struct htable *ht,
		    const struct type_pf_elem *value,
		    gfp_t gfp_flags)
{
	struct slist *n, *prev;
	struct type_pf_elem *data;
	void *tmp;
	int i = 0, j = 0;
	u32 hash = JHASH2(value, h->initval, ht->htable_bits);

	slist_for_each_prev(prev, n, &ht->htable[hash]) {
		for (i = 0; i < h->array_size; i++) {
			data = chash_data(n, i);
			if (type_pf_data_isnull(data)) {
				tmp = n;
				goto found;
			}
		}
		j++;
	}
	if (j < h->chain_limit) {
		tmp = kzalloc(h->array_size * sizeof(struct type_pf_elem)
			      + sizeof(struct slist), gfp_flags);
		if (!tmp)
			return -ENOMEM;
		prev->next = (struct slist *) tmp;
		data = chash_data(tmp, 0);
	} else {
		/* Trigger rehashing */
		return -EAGAIN;
	}
found:
	type_pf_data_copy(data, value);
	return 0;
}

/* Delete an element from the hash table: swap it with the last
 * element in the hash bucket and free up the array if it was
 * completely emptied */
static void
type_pf_chash_del_elem(struct chash *h, struct slist *prev,
		       struct slist *n, int i)
{
	struct type_pf_elem *data = chash_data(n, i);
	struct slist *tmp;
	int j;			/* Index in array */

	if (n->next != NULL) {
		for (prev = n, tmp = n->next;
		     tmp->next != NULL;
		     prev = tmp, tmp = tmp->next)
			/* Find last array */;
		j = 0;
	} else {
		/* Already at last array */
		tmp = n;
		j = i;
	}
	/* Find last non-empty element */
	for (; j < h->array_size - 1; j++)
		if (type_pf_data_isnull(chash_data(tmp, j + 1)))
			break;

	if (!(tmp == n && i == j))
		type_pf_data_swap(data, chash_data(tmp, j));

#ifdef IP_SET_HASH_WITH_NETS
	del_cidr(h, data->cidr, HOST_MASK);
#endif
	if (j == 0) {
		prev->next = NULL;
		kfree(tmp);
	} else
		type_pf_data_zero_out(chash_data(tmp, j));

	h->elements--;
}

/* Resize a hash: create a new hash table with doubling the hashsize
 * and inserting the elements to it. Repeat until we succeed or
 * fail due to memory pressures. */
static int
type_pf_resize(struct ip_set *set, gfp_t gfp_flags, bool retried)
{
	struct chash *h = set->data;
	struct htable *ht, *orig = h->table;
	u8 htable_bits = orig->htable_bits;
	struct slist *n;
	const struct type_pf_elem *data;
	u32 i, j;
	int ret;

retry:
	ret = i = 0;
	htable_bits++;
	if (!htable_bits)
		/* In case we have plenty of memory :-) */
		return -IPSET_ERR_HASH_FULL;
	ht = ip_set_alloc(sizeof(*ht)
			  + jhash_size(htable_bits) * sizeof(struct slist),
			  GFP_KERNEL);
	if (!ht)
		return -ENOMEM;
	ht->htable_bits = htable_bits;

	read_lock_bh(&set->lock);
next_slot:
	for (; i < jhash_size(orig->htable_bits); i++) {
		slist_for_each(n, &orig->htable[i]) {
			for (j = 0; j < h->array_size; j++) {
				data = chash_data(n, j);
				if (type_pf_data_isnull(data)) {
					i++;
					goto next_slot;
				}
				ret = type_pf_chash_readd(h, ht,
							  data, gfp_flags);
				if (ret < 0) {
					read_unlock_bh(&set->lock);
					chash_destroy(ht);
					if (ret == -EAGAIN)
						goto retry;
					return ret;
				}
			}
		}
	}

	h->table = ht;
	read_unlock_bh(&set->lock);

	/* Give time to other users of the set */
	synchronize_net();

	chash_destroy(orig);

	return 0;
}

/* Add an element to a hash and update the internal counters when succeeded,
 * otherwise report the proper error code. */
static int
type_pf_chash_add(struct ip_set *set, void *value,
		  gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	const struct type_pf_elem *d = value;
	struct slist *n, *prev;
	struct htable *ht = h->table;
	struct type_pf_elem *data;
	void *tmp;
	int i = 0, j = 0;
	u32 hash;

	if (h->elements >= h->maxelem)
		return -IPSET_ERR_HASH_FULL;

	hash = JHASH2(value, h->initval, ht->htable_bits);
	slist_for_each_prev(prev, n, &ht->htable[hash]) {
		for (i = 0; i < h->array_size; i++) {
			data = chash_data(n, i);
			if (type_pf_data_isnull(data)) {
				tmp = n;
				goto found;
			}
			if (type_pf_data_equal(data, d))
				return -IPSET_ERR_EXIST;
		}
		j++;
	}
	if (j < h->chain_limit) {
		tmp = kzalloc(h->array_size * sizeof(struct type_pf_elem)
			      + sizeof(struct slist), gfp_flags);
		if (!tmp)
			return -ENOMEM;
		prev->next = (struct slist *) tmp;
		data = chash_data(tmp, 0);
	} else {
		/* Rehashing */
		return -EAGAIN;
	}
found:
	type_pf_data_copy(data, d);
#ifdef IP_SET_HASH_WITH_NETS
	add_cidr(h, d->cidr, HOST_MASK);
#endif
	h->elements++;
	return 0;
}

/* Delete an element from the hash */
static int
type_pf_chash_del(struct ip_set *set, void *value,
		  gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	const struct type_pf_elem *d = value;
	struct htable *ht = h->table;
	struct slist *n, *prev;
	int i;
	struct type_pf_elem *data;
	u32 hash = JHASH2(value, h->initval, ht->htable_bits);

	slist_for_each_prev(prev, n, &ht->htable[hash])
		for (i = 0; i < h->array_size; i++) {
			data = chash_data(n, i);
			if (type_pf_data_isnull(data))
				return -IPSET_ERR_EXIST;
			if (type_pf_data_equal(data, d)) {
				type_pf_chash_del_elem(h, prev, n, i);
				return 0;
			}
		}

	return -IPSET_ERR_EXIST;
}

#ifdef IP_SET_HASH_WITH_NETS

/* Special test function which takes into account the different network
 * sizes added to the set */
static inline int
type_pf_chash_test_cidrs(struct ip_set *set,
			 struct type_pf_elem *d,
			 gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	struct htable *ht = h->table;
	struct slist *n;
	const struct type_pf_elem *data;
	int i, j = 0;
	u32 hash;
	u8 host_mask = SET_HOST_MASK(set->family);

retry:
	pr_debug("test by nets");
	for (; j < host_mask && h->nets[j].cidr; j++) {
		type_pf_data_netmask(d, h->nets[j].cidr);
		hash = JHASH2(d, h->initval, ht->htable_bits);
		slist_for_each(n, &ht->htable[hash])
			for (i = 0; i < h->array_size; i++) {
				data = chash_data(n, i);
				if (type_pf_data_isnull(data)) {
					j++;
					goto retry;
				}
				if (type_pf_data_equal(data, d))
					return 1;
			}
	}
	return 0;
}
#endif

/* Test whether the element is added to the set */
static inline int
type_pf_chash_test(struct ip_set *set, void *value,
		   gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	struct htable *ht = h->table;
	struct type_pf_elem *d = value;
	struct slist *n;
	const struct type_pf_elem *data;
	int i;
	u32 hash;

#ifdef IP_SET_HASH_WITH_NETS
	/* If we test an IP address and not a network address,
	 * try all possible network sizes */
	if (d->cidr == SET_HOST_MASK(set->family))
		return type_pf_chash_test_cidrs(set, d, gfp_flags, timeout);
#endif

	hash = JHASH2(d, h->initval, ht->htable_bits);
	slist_for_each(n, &ht->htable[hash])
		for (i = 0; i < h->array_size; i++) {
			data = chash_data(n, i);
			if (type_pf_data_isnull(data))
				return 0;
			if (type_pf_data_equal(data, d))
				return 1;
		}
	return 0;
}

/* Reply a HEADER request: fill out the header part of the set */
static int
type_pf_head(struct ip_set *set, struct sk_buff *skb)
{
	const struct chash *h = set->data;
	struct nlattr *nested;
	size_t memsize;

	read_lock_bh(&set->lock);
	memsize = chash_memsize(h, with_timeout(h->timeout)
					? sizeof(struct type_pf_telem)
					: sizeof(struct type_pf_elem),
				set->family == AF_INET ? 32 : 128);
	read_unlock_bh(&set->lock);

	nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
	NLA_PUT_NET32(skb, IPSET_ATTR_HASHSIZE,
		      htonl(jhash_size(h->table->htable_bits)));
	NLA_PUT_NET32(skb, IPSET_ATTR_MAXELEM, htonl(h->maxelem));
#ifdef IP_SET_HASH_WITH_NETMASK
	if (h->netmask != HOST_MASK)
		NLA_PUT_U8(skb, IPSET_ATTR_NETMASK, h->netmask);
#endif
	NLA_PUT_NET32(skb, IPSET_ATTR_REFERENCES,
		      htonl(atomic_read(&set->ref) - 1));
	NLA_PUT_NET32(skb, IPSET_ATTR_MEMSIZE, htonl(memsize));
	if (with_timeout(h->timeout))
		NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT, htonl(h->timeout));
	ipset_nest_end(skb, nested);

	return 0;
nla_put_failure:
	return -EFAULT;
}

/* Reply a LIST/SAVE request: dump the elements of the specified set */
static int
type_pf_list(struct ip_set *set,
	     struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct chash *h = set->data;
	const struct htable *ht = h->table;
	struct nlattr *atd, *nested;
	struct slist *n;
	const struct type_pf_elem *data;
	u32 first = cb->args[2];
	/* We assume that one hash bucket fills into one page */
	void *incomplete;
	int i;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EFAULT;
	pr_debug("list hash set %s", set->name);
	for (; cb->args[2] < jhash_size(ht->htable_bits); cb->args[2]++) {
		incomplete = skb_tail_pointer(skb);
		slist_for_each(n, &ht->htable[cb->args[2]]) {
			for (i = 0; i < h->array_size; i++) {
				data = chash_data(n, i);
				if (type_pf_data_isnull(data))
					break;
				pr_debug("list hash %lu slist %p i %u",
					 cb->args[2], n, i);
				nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
				if (!nested) {
					if (cb->args[2] == first) {
						nla_nest_cancel(skb, atd);
						return -EFAULT;
					} else
						goto nla_put_failure;
				}
				if (type_pf_data_list(skb, data))
					goto nla_put_failure;
				ipset_nest_end(skb, nested);
			}
		}
	}
	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[2] = 0;

	return 0;

nla_put_failure:
	nlmsg_trim(skb, incomplete);
	ipset_nest_end(skb, atd);
	if (unlikely(first == cb->args[2])) {
		pr_warn("Can't list set %s: one bucket does not fit into "
			"a message. Please report it!\n", set->name);
		cb->args[2] = 0;
	}
	return 0;
}

static int
type_pf_kadt(struct ip_set *set, const struct sk_buff * skb,
	     enum ipset_adt adt, u8 pf, u8 dim, u8 flags);
static int
type_pf_uadt(struct ip_set *set, struct nlattr *head, int len,
	     enum ipset_adt adt, u32 *lineno, u32 flags);

static const struct ip_set_type_variant type_pf_variant = {
	.kadt	= type_pf_kadt,
	.uadt	= type_pf_uadt,
	.adt	= {
		[IPSET_ADD] = type_pf_chash_add,
		[IPSET_DEL] = type_pf_chash_del,
		[IPSET_TEST] = type_pf_chash_test,
	},
	.destroy = type_pf_destroy,
	.flush	= type_pf_flush,
	.head	= type_pf_head,
	.list	= type_pf_list,
	.resize	= type_pf_resize,
	.same_set = type_pf_same_set,
};

/* Flavour with timeout support */

#define chash_tdata(n, i) \
(struct type_pf_elem *)((char *)(n) + sizeof(struct slist) \
				    + (i)*sizeof(struct type_pf_telem))

static inline u32
type_pf_data_timeout(const struct type_pf_elem *data)
{
	const struct type_pf_telem *tdata =
		(const struct type_pf_telem *) data;

	return tdata->timeout;
}

static inline bool
type_pf_data_expired(const struct type_pf_elem *data)
{
	const struct type_pf_telem *tdata =
		(const struct type_pf_telem *) data;

	return ip_set_timeout_expired(tdata->timeout);
}

static inline void
type_pf_data_swap_timeout(struct type_pf_elem *src,
			  struct type_pf_elem *dst)
{
	struct type_pf_telem *x = (struct type_pf_telem *) src;
	struct type_pf_telem *y = (struct type_pf_telem *) dst;

	swap(x->timeout, y->timeout);
}

static inline void
type_pf_data_timeout_set(struct type_pf_elem *data, u32 timeout)
{
	struct type_pf_telem *tdata = (struct type_pf_telem *) data;

	tdata->timeout = ip_set_timeout_set(timeout);
}

static int
type_pf_chash_treadd(struct chash *h, struct htable *ht,
		     const struct type_pf_elem *value,
		     gfp_t gfp_flags, u32 timeout)
{
	struct slist *n, *prev;
	struct type_pf_elem *data;
	void *tmp;
	int i = 0, j = 0;
	u32 hash = JHASH2(value, h->initval, ht->htable_bits);

	slist_for_each_prev(prev, n, &ht->htable[hash]) {
		for (i = 0; i < h->array_size; i++) {
			data = chash_tdata(n, i);
			if (type_pf_data_isnull(data)) {
				tmp = n;
				goto found;
			}
		}
		j++;
	}
	if (j < h->chain_limit) {
		tmp = kzalloc(h->array_size * sizeof(struct type_pf_telem)
			      + sizeof(struct slist), gfp_flags);
		if (!tmp)
			return -ENOMEM;
		prev->next = (struct slist *) tmp;
		data = chash_tdata(tmp, 0);
	} else {
		/* Trigger rehashing */
		return -EAGAIN;
	}
found:
	type_pf_data_copy(data, value);
	type_pf_data_timeout_set(data, timeout);
	return 0;
}

static void
type_pf_chash_del_telem(struct chash *h, struct slist *prev,
			struct slist *n, int i)
{
	struct type_pf_elem *d, *data = chash_tdata(n, i);
	struct slist *tmp;
	int j;		/* Index in array */

	pr_debug("del %u", i);
	if (n->next != NULL) {
		for (prev = n, tmp = n->next;
		     tmp->next != NULL;
		     prev = tmp, tmp = tmp->next)
			/* Find last array */;
		j = 0;
	} else {
		/* Already at last array */
		tmp = n;
		j = i;
	}
	/* Find last non-empty element */
	for (; j < h->array_size - 1; j++)
		if (type_pf_data_isnull(chash_tdata(tmp, j + 1)))
			break;

	d = chash_tdata(tmp, j);
	if (!(tmp == n && i == j)) {
		type_pf_data_swap(data, d);
		type_pf_data_swap_timeout(data, d);
	}
#ifdef IP_SET_HASH_WITH_NETS
	del_cidr(h, data->cidr, HOST_MASK);
#endif
	if (j == 0) {
		prev->next = NULL;
		kfree(tmp);
	} else
		type_pf_data_zero_out(d);

	h->elements--;
}

/* Delete expired elements from the hashtable */
static void
type_pf_chash_expire(struct chash *h)
{
	struct htable *ht = h->table;
	struct slist *n, *prev;
	struct type_pf_elem *data;
	u32 i;
	int j;

	for (i = 0; i < jhash_size(ht->htable_bits); i++)
		slist_for_each_prev(prev, n, &ht->htable[i])
			for (j = 0; j < h->array_size; j++) {
				data = chash_tdata(n, j);
				if (type_pf_data_isnull(data))
					break;
				if (type_pf_data_expired(data)) {
					pr_debug("expire %u/%u", i, j);
					type_pf_chash_del_telem(h, prev, n, j);
				}
			}
}

static int
type_pf_tresize(struct ip_set *set, gfp_t gfp_flags, bool retried)
{
	struct chash *h = set->data;
	struct htable *ht, *orig = h->table;
	u8 htable_bits = orig->htable_bits;
	struct slist *n;
	const struct type_pf_elem *data;
	u32 i, j;
	int ret;

	/* Try to cleanup once */
	if (!retried) {
		i = h->elements;
		write_lock_bh(&set->lock);
		type_pf_chash_expire(set->data);
		write_unlock_bh(&set->lock);
		if (h->elements <  i)
			return 0;
	}

retry:
	ret = i = 0;
	htable_bits++;
	if (!htable_bits)
		/* In case we have plenty of memory :-) */
		return -IPSET_ERR_HASH_FULL;
	ht = ip_set_alloc(sizeof(*ht)
			 + jhash_size(htable_bits) * sizeof(struct slist),
			 GFP_KERNEL);
	if (!ht)
		return -ENOMEM;
	ht->htable_bits = htable_bits;

	read_lock_bh(&set->lock);
next_slot:
	for (; i < jhash_size(orig->htable_bits); i++) {
		slist_for_each(n, &orig->htable[i]) {
			for (j = 0; j < h->array_size; j++) {
				data = chash_tdata(n, j);
				if (type_pf_data_isnull(data)) {
					i++;
					goto next_slot;
				}
				ret = type_pf_chash_treadd(h, ht,
						data, gfp_flags,
						type_pf_data_timeout(data));
				if (ret < 0) {
					read_unlock_bh(&set->lock);
					chash_destroy(ht);
					if (ret == -EAGAIN)
						goto retry;
					return ret;
				}
			}
		}
	}

	h->table = ht;
	read_unlock_bh(&set->lock);

	/* Give time to other users of the set */
	synchronize_net();

	chash_destroy(orig);

	return 0;
}

static int
type_pf_chash_tadd(struct ip_set *set, void *value,
		   gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	const struct type_pf_elem *d = value;
	struct slist *n, *prev;
	struct htable *ht = h->table;
	struct type_pf_elem *data;
	void *tmp;
	int i = 0, j = 0;
	u32 hash;

	if (h->elements >= h->maxelem)
		/* FIXME: when set is full, we slow down here */
		type_pf_chash_expire(h);
	if (h->elements >= h->maxelem)
		return -IPSET_ERR_HASH_FULL;

	hash = JHASH2(d, h->initval, ht->htable_bits);
	slist_for_each_prev(prev, n, &ht->htable[hash]) {
		for (i = 0; i < h->array_size; i++) {
			data = chash_tdata(n, i);
			if (type_pf_data_isnull(data)
			    || type_pf_data_expired(data)) {
				tmp = n;
				goto found;
			}
			if (type_pf_data_equal(data, d))
				return -IPSET_ERR_EXIST;
		}
		j++;
	}
	if (j < h->chain_limit) {
		tmp = kzalloc(h->array_size * sizeof(struct type_pf_telem)
			      + sizeof(struct slist), gfp_flags);
		if (!tmp)
			return -ENOMEM;
		prev->next = (struct slist *) tmp;
		data = chash_tdata(tmp, 0);
	} else {
		/* Rehashing */
		return -EAGAIN;
	}
found:
	if (type_pf_data_isnull(data))
		h->elements++;
#ifdef IP_SET_HASH_WITH_NETS
	else
		del_cidr(h, data->cidr, HOST_MASK);

	add_cidr(h, d->cidr, HOST_MASK);
#endif
	type_pf_data_copy(data, d);
	type_pf_data_timeout_set(data, timeout);
	return 0;
}

static int
type_pf_chash_tdel(struct ip_set *set, void *value,
		   gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	struct htable *ht = h->table;
	const struct type_pf_elem *d = value;
	struct slist *n, *prev;
	int i, ret = 0;
	struct type_pf_elem *data;
	u32 hash = JHASH2(value, h->initval, ht->htable_bits);

	slist_for_each_prev(prev, n, &ht->htable[hash])
		for (i = 0; i < h->array_size; i++) {
			data = chash_tdata(n, i);
			if (type_pf_data_isnull(data))
				return -IPSET_ERR_EXIST;
			if (type_pf_data_equal(data, d)) {
				if (type_pf_data_expired(data))
					ret = -IPSET_ERR_EXIST;
				type_pf_chash_del_telem(h, prev, n, i);
				return ret;
			}
		}

	return -IPSET_ERR_EXIST;
}

#ifdef IP_SET_HASH_WITH_NETS
static inline int
type_pf_chash_ttest_cidrs(struct ip_set *set,
			  struct type_pf_elem *d,
			  gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	struct htable *ht = h->table;
	struct type_pf_elem *data;
	struct slist *n;
	int i, j = 0;
	u32 hash;
	u8 host_mask = SET_HOST_MASK(set->family);

retry:
	for (; j < host_mask && h->nets[j].cidr; j++) {
		type_pf_data_netmask(d, h->nets[j].cidr);
		hash = JHASH2(d, h->initval, ht->htable_bits);
		slist_for_each(n, &ht->htable[hash])
			for (i = 0; i < h->array_size; i++) {
				data = chash_tdata(n, i);
				if (type_pf_data_isnull(data)) {
					j++;
					goto retry;
				}
				if (type_pf_data_equal(data, d))
					return !type_pf_data_expired(data);
			}
	}
	return 0;
}
#endif

static inline int
type_pf_chash_ttest(struct ip_set *set, void *value,
		    gfp_t gfp_flags, u32 timeout)
{
	struct chash *h = set->data;
	struct htable *ht = h->table;
	struct type_pf_elem *data, *d = value;
	struct slist *n;
	int i;
	u32 hash;

#ifdef IP_SET_HASH_WITH_NETS
	if (d->cidr == SET_HOST_MASK(set->family))
		return type_pf_chash_ttest_cidrs(set, d, gfp_flags,
						 timeout);
#endif
	hash = JHASH2(d, h->initval, ht->htable_bits);
	slist_for_each(n, &ht->htable[hash])
		for (i = 0; i < h->array_size; i++) {
			data = chash_tdata(n, i);
			if (type_pf_data_isnull(data))
				return 0;
			if (type_pf_data_equal(data, d))
				return !type_pf_data_expired(data);
		}
	return 0;
}

static int
type_pf_tlist(struct ip_set *set,
	      struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct chash *h = set->data;
	const struct htable *ht = h->table;
	struct nlattr *atd, *nested;
	struct slist *n;
	const struct type_pf_elem *data;
	u32 first = cb->args[2];
	/* We assume that one hash bucket fills into one page */
	void *incomplete;
	int i;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EFAULT;
	for (; cb->args[2] < jhash_size(ht->htable_bits); cb->args[2]++) {
		incomplete = skb_tail_pointer(skb);
		slist_for_each(n, &ht->htable[cb->args[2]]) {
			for (i = 0; i < h->array_size; i++) {
				data = chash_tdata(n, i);
				pr_debug("list %p %u", n, i);
				if (type_pf_data_isnull(data))
					break;
				if (type_pf_data_expired(data))
					continue;
				pr_debug("do list %p %u", n, i);
				nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
				if (!nested) {
					if (cb->args[2] == first) {
						nla_nest_cancel(skb, atd);
						return -EFAULT;
					} else
						goto nla_put_failure;
				}
				if (type_pf_data_tlist(skb, data))
					goto nla_put_failure;
				ipset_nest_end(skb, nested);
			}
		}
	}
	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[2] = 0;

	return 0;

nla_put_failure:
	nlmsg_trim(skb, incomplete);
	ipset_nest_end(skb, atd);
	if (unlikely(first == cb->args[2])) {
		pr_warn("Can't list set %s: one bucket does not fit into "
			"a message. Please report it!\n", set->name);
		cb->args[2] = 0;
	}
	return 0;
}

static const struct ip_set_type_variant type_pf_tvariant = {
	.kadt	= type_pf_kadt,
	.uadt	= type_pf_uadt,
	.adt	= {
		[IPSET_ADD] = type_pf_chash_tadd,
		[IPSET_DEL] = type_pf_chash_tdel,
		[IPSET_TEST] = type_pf_chash_ttest,
	},
	.destroy = type_pf_destroy,
	.flush	= type_pf_flush,
	.head	= type_pf_head,
	.list	= type_pf_tlist,
	.resize	= type_pf_tresize,
	.same_set = type_pf_same_set,
};

static void
type_pf_gc(unsigned long ul_set)
{
	struct ip_set *set = (struct ip_set *) ul_set;
	struct chash *h = set->data;

	pr_debug("called");
	write_lock_bh(&set->lock);
	type_pf_chash_expire(h);
	write_unlock_bh(&set->lock);

	h->gc.expires = jiffies + IPSET_GC_PERIOD(h->timeout) * HZ;
	add_timer(&h->gc);
}

static inline void
type_pf_gc_init(struct ip_set *set)
{
	struct chash *h = set->data;

	init_timer(&h->gc);
	h->gc.data = (unsigned long) set;
	h->gc.function = type_pf_gc;
	h->gc.expires = jiffies + IPSET_GC_PERIOD(h->timeout) * HZ;
	add_timer(&h->gc);
	pr_debug("gc initialized, run in every %u",
		 IPSET_GC_PERIOD(h->timeout));
}

#undef type_pf_data_equal
#undef type_pf_data_isnull
#undef type_pf_data_copy
#undef type_pf_data_swap
#undef type_pf_data_zero_out
#undef type_pf_data_list
#undef type_pf_data_tlist

#undef type_pf_elem
#undef type_pf_telem
#undef type_pf_data_timeout
#undef type_pf_data_expired
#undef type_pf_data_swap_timeout
#undef type_pf_data_netmask
#undef type_pf_data_timeout_set

#undef type_pf_chash_readd
#undef type_pf_chash_del_elem
#undef type_pf_chash_add
#undef type_pf_chash_del
#undef type_pf_chash_test_cidrs
#undef type_pf_chash_test

#undef type_pf_chash_treadd
#undef type_pf_chash_del_telem
#undef type_pf_chash_expire
#undef type_pf_chash_tadd
#undef type_pf_chash_tdel
#undef type_pf_chash_ttest_cidrs
#undef type_pf_chash_ttest

#undef type_pf_resize
#undef type_pf_tresize
#undef type_pf_flush
#undef type_pf_destroy
#undef type_pf_head
#undef type_pf_list
#undef type_pf_tlist
#undef type_pf_same_set
#undef type_pf_kadt
#undef type_pf_uadt
#undef type_pf_gc
#undef type_pf_gc_init
#undef type_pf_variant
#undef type_pf_tvariant
