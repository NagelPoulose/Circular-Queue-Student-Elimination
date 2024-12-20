#include <stdio.h>
#include <stdlib.h>

// This program is written by: Nagel Poulose

// Student struct to create student nodes
typedef struct student {
  int studentnum;
  struct student *next;
} student;

// Queue struct to implement the queue
typedef struct queue {
  student *f;
  student *b;
  int th;
  int k;
  int nodecount;
} queue;

// Creating 10 queues as max amount of queues is 10
queue q[10];

// Creates a student node
student *createStudent(int sequence) {
  student *newstudent = malloc(sizeof(student));
  newstudent->studentnum = sequence;
  newstudent->next = NULL;
  return newstudent;
}

// Adds student node into the specific queue
void enqueue(queue *q, student *newstudent) {
  if (q->b != NULL) {
    q->b->next = newstudent;
  }
  q->b = newstudent;

  if (q->f == NULL) {
    q->f = newstudent;
  }
  q->b->next = q->f;
  q->nodecount++;
}

// Creates a circular singly linked list of students while referencing a queue
void createReverseCircle(queue *q) {
  int studentnum, k, th;
  scanf("%d %d %d", &studentnum, &k, &th);
  q->k = k;
  q->th = th;
  for (int i = studentnum; i > 0; i--) {
    student *temp = createStudent(i);
    enqueue(q, temp);
  }
}

// Function initializes the queue structure
void init(queue *q) {
  q->f = NULL;
  q->b = NULL;
  q->th = 0;
  q->k = 0;
  q->nodecount = 0;
}

// Checks if a queue is empty
int isempty(queue *q) { return q->f == NULL; }

// Display queue function
void display(queue *q) {
  for (int i = 0; i < 10; i++) {
    if (!isempty(&q[i])) {
      printf("\n%d", i + 1);

      student *t = q[i].f; // Starts at the front of the queue

      do {
        printf(" %d", t->studentnum);
        t = t->next;
      } while (t != q[i].f); // the loop ends when we circle back to the front
    }
  }
  printf("\n");
}

// Reverses the circular linked list of the given queue to make the list in
// ascending order
void rearrangeCircle(queue *q) {

  if (q->f == NULL || q->f == q->b) {
    return; // No need to reverse if empty or only one node
  }

  student *prev = NULL;
  student *curr = q->f;
  student *next = NULL;
  student *first = q->f;

  q->b->next = NULL;

  while (curr != NULL) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }

  q->f = prev;
  q->b = first;

  q->b->next = q->f;
}

void phase1elim(queue *q) {
  printf("\nPhase1 elimination\n");

  // Loop through the 10 queues
  for (int i = 0; i < 10; i++) {
    if (!isempty(&q[i])) {
      printf("\nGroup for Garage# %d\n", i + 1);

      student *walker = q[i].f;

      // Continues the elimination process until nodecount is <= threshold 
      while ((q[i].nodecount > q[i].th)){ 

        for (int j = 0; j < q[i].k - 2; j++) {
          walker = walker->next;
        }

        // prints the node that is being eliminated
        student *store = walker->next;
        printf("Student# %d eliminated\n", store->studentnum);

        if (store == q[i].f) {
          q[i].f = store->next;
          q[i].b->next = q[i].f;
        }

        else if (store == q[i].b) {
          q[i].b = walker;
          q[i].b->next = q[i].f;
        }

        else {
          walker->next = store->next;
        }

        free(store);
        q[i].nodecount--;

        walker = walker->next;
      }
    }
  }
}

// function just returns the node at the front of the queue
int peek(queue *q) {
  if (q->f != NULL) {
    return q->f->studentnum;
  } else {
    return -1;
  }
}

// funtion frees the front node and moves the front node to the next node
void dequeue(queue *q) {
  if (q->f == NULL) {

    return;
  }

  if (q->f == q->b) {
    free(q->f);
    q->f = NULL;
    q->b = NULL;
  } else {

    student *temp = q->f;
    q->f = q->f->next;
    q->b->next = q->f;
    free(temp);
  }

  q->nodecount--;
}

// function keeps track of total amount of nodes across all queues
int totstu(queue *q) {
  int total = 0;
  for (int i = 0; i < 10; i++) {
    if (!isempty(&q[i])) {
      total = q[i].nodecount + total;
    }
  }
  return total;
}

// function does second round of eliminations til there is only one node left
void phase2elim(queue *q, int total) {
  printf("\nPhase2 elimination\n");
  int temp = -1;
  int a = total;

  while (a > 1) {
    temp = -1;
    for (int i = 0; i < 10; i++) {
      if (!isempty(&q[i])) {
        if (peek(&q[i]) > temp) {
          temp = peek(&q[i]);
        }
      }
    }

    // Eliminates the student with the largest number from their respective
    // queue
    for (int i = 0; i < 10; i++) {
      if (!isempty(&q[i])) {
        if (peek(&q[i]) == temp) {
          printf("Eliminated student %d from group for garage %d\n", temp,
                 i + 1);
          dequeue(&q[i]);
          a--;
          break;
        }
      }
    }
  }
  // prints out the winner and their garage number
  for (int i = 0; i < 10; i++) {
    if (!isempty(&q[i])) {
      printf("\nStudent %d from the group for garage %d is the winner!",
             peek(&q[i]), (i + 1));
      dequeue(&q[i]);
    }
  }
}

int main() {
  // Initializes the 10 queues
  int G, gnum;
  scanf("%d", &G);
  for (int i = 0; i < 10; i++) {
    init(&q[i]);
  }

  // Array to track which queues are created
  int created_queues[G];

  // Creating the reversed list for each garage
  for (int i = 0; i < G; i++) {
    scanf("%d", &gnum);
    createReverseCircle(&q[gnum - 1]);
    created_queues[i] = gnum - 1;
  }

  // Displays the initial status of nonempty queues
  printf("Initial status of nonempty queues");
  display(q);

  // Rearrange the circular list for each created queue to make it ascending
  // order
  for (int i = 0; i < G; i++) {
    rearrangeCircle(&q[created_queues[i]]);
  }

  // Display the queues after rearranging
  printf("\nAfter ordering status of nonempty queues");
  display(q);

  // Perform phase 1 elimination
  phase1elim(q);

  // Perform phase 2 elimination
  int t;
  t = totstu(q);
  phase2elim(q, t);

  return 0;
}