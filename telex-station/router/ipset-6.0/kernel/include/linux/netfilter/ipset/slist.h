#ifndef _IP_SET_SLIST_H
#define _IP_SET_SLIST_H

#include <linux/stddef.h>
#include <linux/prefetch.h>
#include <asm/system.h>

/*
 * Single linked lists with a single pointer.
 * Mostly useful for hash tables where the two pointer list head
 * and list node is too wasteful.
 */

struct slist {
	struct slist *next;
};

#define SLIST(name) struct slist name = {  .next = NULL }
#define INIT_SLIST(ptr) ((ptr)->next = NULL)

#define slist_entry(ptr, type, member) container_of(ptr, type, member)

#define slist_for_each(pos, head) \
	for (pos = (head)->next; pos && ({ prefetch(pos->next); 1; }); \
	     pos = pos->next)

#define slist_for_each_prev(prev, pos, head) \
	for (prev = head, pos = (head)->next; \
	     pos && ({ prefetch(pos->next); 1; }); \
	     prev = pos, pos = pos->next)

#define slist_for_each_safe(pos, n, head) \
	for (pos = (head)->next; pos && ({ n = pos->next; 1; }); \
	     pos = n)

/**
 * slist_for_each_entry	- iterate over list of given type
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct slist to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the slist within the struct.
 */
#define slist_for_each_entry(tpos, pos, head, member)			 \
	for (pos = (head)->next;					 \
	     pos && ({ prefetch(pos->next); 1; }) &&			 \
		({ tpos = slist_entry(pos, typeof(*tpos), member); 1; });\
	     pos = pos->next)

/**
 * slist_for_each_entry_continue - iterate over a hlist continuing
 *				   after current point
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct slist to use as a loop cursor.
 * @member:	the name of the slist within the struct.
 */
#define slist_for_each_entry_continue(tpos, pos, member)		 \
	for (pos = (pos)->next;						 \
	     pos && ({ prefetch(pos->next); 1; }) &&			 \
		({ tpos = slist_entry(pos, typeof(*tpos), member); 1; });\
	     pos = pos->next)

/**
 * slist_for_each_entry_from - iterate over a hlist continuing
 *			       from current point
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct slist to use as a loop cursor.
 * @member:	the name of the slist within the struct.
 */
#define slist_for_each_entry_from(tpos, pos, member)			 \
	for (; pos && ({ prefetch(pos->next); 1; }) &&			 \
		({ tpos = slist_entry(pos, typeof(*tpos), member); 1; });\
	     pos = pos->next)

/**
 * slist_for_each_entry_safe - iterate over list of given type safe against
 *			       removal of list entry
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct slist to use as a loop cursor.
 * @n:		another &struct slist to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the slist within the struct.
 */
#define slist_for_each_entry_safe(tpos, pos, n, head, member)		\
	for (pos = (head)->next;					\
	     pos && ({ n = pos->next; 1; }) &&				\
		({ tpos = slist_entry(pos, typeof(*tpos), member); 1; });\
	     pos = n)

#endif /* _IP_SET_SLIST_H */
