#include "../include/pointset.h"

struct pointset *pointset_new()
{
	return NULL;
}

void pointset_free(struct pointset **set)
{
	if (*set == NULL)
		return;

	vector3_free(&(*set)->point);
	(*set)->prev->next = NULL;
	struct pointset *aux = (*set)->next;
	free(*set);
	pointset_free(&aux);

	*set = NULL;
}

struct vector3 *pointset_insert_head(struct pointset **set,
																		 real x,
																		 real y,
																		 real z)
{
	struct pointset *new = malloc(sizeof(struct pointset));
	if (new == NULL)
		return NULL;

	new->point = vector3_new(x, y, z);
	if (new->point == NULL)
	{
		free(new);
		return NULL;
	}

	if (*set != NULL)
	{
		new->next = *set;
		new->prev = (*set)->prev;

		(*set)->prev = new;
		new->prev->next = new;
	}
	else
	{
		new->next = new;
		new->prev = new;
	}

	*set = new;
	return new->point;
}

struct vector3 *pointset_insert_tail(struct pointset **set,
																		 real x,
																		 real y,
																		 real z)
{
	struct vector3 *v3_ptr = pointset_insert_head(set, x, y, z);

	*set = (*set)->next;

	return v3_ptr;
}

struct vector3 *pointset_insert_next(struct pointset **set,
																		 uint i,
																		 real x,
																		 real y,
																		 real z)
{
	struct pointset *aux_set = *set;
	for (uint j = 0; j <= i; j++)
	{
		aux_set = aux_set->next;
	}

	return pointset_insert_head(&aux_set, x, y, z);
}

struct vector3 *pointset_insert_prev(struct pointset **set,
																		 uint i,
																		 real x,
																		 real y,
																		 real z)
{
	struct pointset *aux_set = *set;
	for (uint j = 0; j < i; j++)
	{
		aux_set = aux_set->next;
	}

	pointset_insert_head(&aux_set, x, y, z);

	if (aux_set->next == *set)
	{
		*set = aux_set;
	}

	return aux_set->point;
}

void pointset_debug(struct pointset *set, FILE *output)
{
	struct pointset *s = set;
	if (!s)
	{
		fprintf(output, "!!! pointset empty !!!\n");
		return;
	}
	do
	{
		vector3_debug(s->point, output);
		s = s->next;
	} while (s != NULL && s != set);
}

void pointset_debug_reverse(struct pointset *set, FILE *output)
{
	struct pointset *s = set;
	if (!s)
	{
		fprintf(output, "!!! pointset empty !!!\n");
		return;
	}
	s = s->prev;
	do
	{
		vector3_debug(s->point, output);
		s = s->prev;
	} while (s != NULL && s != set->prev);
}

struct pointset *pointset_copy(struct pointset *set)
{
	struct pointset *cpy = pointset_new();
	struct pointset *tmp = set->prev;
	do
	{
		pointset_insert_head(&cpy, tmp->point->x, tmp->point->y, tmp->point->z);
		tmp = tmp->prev;
	} while (tmp != NULL && tmp != set->prev);

	return cpy;
}

struct pointset *pointset_segment(struct pointset *begin,
																	struct pointset *end,
																	uint *size)
{
	struct pointset *segment = pointset_new();

	if (begin == end)
	{
		pointset_insert_head(&segment,
												 begin->point->x,
												 begin->point->y,
												 begin->point->z);
		*size = 1;
		return segment;
	}

	struct pointset *set = begin;
	do
	{
		pointset_insert_tail(&segment, set->point->x, set->point->y, set->point->z);
		*size = *size + 1;
		set = set->next;
	} while (set != NULL && set != begin && set->prev != end);

	return segment;
}

struct pointset *pointset_segment_reverse(struct pointset *begin,
																					struct pointset *end,
																					uint *size)
{
	struct pointset *segment = pointset_new();
	
	if (begin == end)
	{
		pointset_insert_head(&segment,
												 begin->point->x,
												 begin->point->y,
												 begin->point->z);
		*size = 1;
		return segment;
	}

	struct pointset *set = begin;
	do
	{
		pointset_insert_tail(&segment, set->point->x, set->point->y, set->point->z);
		*size = *size + 1;
		set = set->prev;
	} while (set != NULL && set != begin && set->next != end);

	return segment;
}

uint pointset_size(struct pointset *set)
{
	uint size = 0;

	struct pointset *s = set;
	if (!s)	{
		return size;
	}

	do {
		s = s->next;
		size++;
	} while (s != NULL && s != set);

	return size;
}

void pointset_swap(struct vector3 **a, struct vector3 **b)
{
	struct vector3 *tmp = *a;

	*a = *b;
	*b = tmp;
}


void pointset_recursive_sort(struct pointset *head,
														 struct pointset *tail,
														 int axis)
{
	struct pointset *pivot = head;
	char to_left = 1;

	if (head == NULL || tail == NULL || head == tail) return;

	struct pointset *i = tail;


	while (i != pivot) {
		if (to_left) {
			if (pivot->point->coord[axis] > i->point->coord[axis]) {
				pointset_swap(&(pivot->point), &(i->point));
				struct pointset *aux = pivot->next;
				pivot = i;
				i = aux;
				to_left = 0;
			}
			else {
				i = i->prev;
			}
		}
		else {
			if (pivot->point->coord[axis] < i->point->coord[axis]) {
				pointset_swap(&(pivot->point), &(i->point));
				struct pointset *aux = pivot->prev;
				pivot = i;
				i = aux;
				to_left = 1;
			}
			else {
				i = i->next;
			}
		}
	}

	if (pivot != head) {
		pointset_recursive_sort(head, pivot->prev, axis);
	}

	if (pivot != tail) {
		pointset_recursive_sort(pivot->next, tail, axis);
	}
}

void pointset_sort(struct pointset *set, int axis)
{
	struct pointset *end = set->prev;
	pointset_recursive_sort(set, end, axis);
}

// struct pointset *pointset_median(struct pointset *set, int axis, uint size)
// {
// 	if (set == NULL || size == 0)
// 		return NULL;

// 	if (size == 1)
// 		return set;

// 	pointset_sort(set, axis);

// 	struct pointset *tmp = set;
// 	uint i = 0;
// 	uint h = size / 2;

// 	do {
// 		i++;
// 		tmp = tmp->next;
// 	} while (tmp != NULL && i < h);

// 	if (size % 2 == 0)
// 		return tmp->prev;
// 	else
// 		return tmp;
// }
