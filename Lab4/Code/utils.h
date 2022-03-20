#pragma once

struct node {
  int *inlinks;
  int num_in_links;
  int num_out_links;
};

/**
 * @brief Create tree
 *
 * @param [out] nodehead head of the node
 * @param start Start Node number
 * @param end End node number
 * @return 0 if success, other numbers otherwise
 */
int node_init(struct node **nodehead, int start, int end);

/**
 * @brief Node destructor
 *
 * @param nodehead head of the node
 * @param num_nodes number of nodes to clean up
 */
void node_destroy(struct node *nodehead, int num_nodes);

/**
 * @brief Calculate relative error between two vectors
 *
 * @param r left vector
 * @param t right vector
 * @return double relative error between vectors
 */
double rel_error(double *r, double *t, int size);

/**
 * @brief Copy vector @param source of size @param size to @param dest
 *
 * @param source
 * @param dest
 */
void vec_cp(double *source, double *dest, int size);
