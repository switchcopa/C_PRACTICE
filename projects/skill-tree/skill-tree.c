#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "skill-tree.h"


struct Node {
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    const char* name;
    const char* description;
    int cost;
    bool purchased;
};


typedef struct NodeArgs {
    const char* name;
    const char* description;
    int cost;
} NodeArgs;


struct Node* create_node(NodeArgs args) {
    struct Node* newnode = malloc(sizeof(struct Node));

    if (!newnode) {
        printf("Failed to allocate memory.\n");
        exit(1);
    }
       
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = NULL;
    newnode->name = args.name;
    newnode->description = args.description;
    newnode->cost = args.cost;
    newnode->purchased = false; 

    return newnode;
}


struct Node* init_agility_tree() {
    struct Node* quick_step = create_node((NodeArgs){
        .name = "Quick Step",
        .description = "Basic burst of speed and nimbleness.",
        .cost = 1,
    });

    struct Node* fleet_foot = create_node((NodeArgs){
        .name = "Fleet Foot",
        .description = "Swift evasive movement, almost vanishing in motion.",
        .cost = 2,
    });

    struct Node* shadow_dash = create_node((NodeArgs){
        .name = "Shadow Dash",
        .description = "Swift evasive movement, almost vanishing in motion.",
        .cost = 2,
    });

    struct Node* phantom_stride = create_node((NodeArgs){
        .name = "Phantom Stride",
        .description = "Blindingly fast, leaving afterimages behind.",
        .cost = 3,
    });
    
    struct Node* temporal_shift = create_node((NodeArgs){
        .name = "Temporal Shift",
        .description = "Agility so great it feels like bending time itself.",
        .cost = 3,
    });

    quick_step->left = fleet_foot;
    quick_step->right = shadow_dash;
    fleet_foot->left = phantom_stride;
    shadow_dash->right = temporal_shift;
}
