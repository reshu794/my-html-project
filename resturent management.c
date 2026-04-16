#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50

// Food Menu
struct MenuItem {
    char name[SIZE];
    float price;
};

// Predefined menu
struct MenuItem menu[] = {
    {"Burger", 120.0},
    {"Pizza", 250.0},
    {"Fries", 80.0},
    {"Coke", 50.0},
    {"Pasta", 180.0}
};
int menuSize = sizeof(menu) / sizeof(menu[0]);

// Order node structure
struct Order {
    int orderId;
    char customerName[SIZE];
    int tableNumber;
    char foodItem[SIZE];
    int quantity;
    float totalCost;
    struct Order* next;
};

struct Order* head = NULL;
int nextOrderId = 1;

// Function to find food price
float getPrice(char item[]) {
    for (int i = 0; i < menuSize; i++) {
        if (strcmp(menu[i].name, item) == 0)
            return menu[i].price;
    }
    return -1;
}

// Display menu
void showMenu() {
    printf("\n--- Menu ---\n");
    for (int i = 0; i < menuSize; i++) {
        printf("%d. %s - ₹%.2f\n", i + 1, menu[i].name, menu[i].price);
    }
}

// Add a new order
void addOrder(char customerName[], int tableNumber, char foodItem[], int quantity) {
    float price = getPrice(foodItem);
    if (price == -1) {
        printf("Invalid food item. Please select from the menu.\n");
        return;
    }

    struct Order* newOrder = (struct Order*)malloc(sizeof(struct Order));
    newOrder->orderId = nextOrderId++;
    strcpy(newOrder->customerName, customerName);
    newOrder->tableNumber = tableNumber;
    strcpy(newOrder->foodItem, foodItem);
    newOrder->quantity = quantity;
    newOrder->totalCost = price * quantity;
    newOrder->next = NULL;

    if (head == NULL) {
        head = newOrder;
    } else {
        struct Order* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newOrder;
    }

    printf("Order added! ID: %d, Total: ₹%.2f\n", newOrder->orderId, newOrder->totalCost);
}

// Display all orders
void showOrders() {
    if (head == NULL) {
        printf("No current orders.\n");
        return;
    }

    printf("\n--- Current Orders ---\n");
    struct Order* temp = head;
    while (temp != NULL) {
        printf("Order ID: %d | Name: %s | Table: %d | Item: %s | Qty: %d | Total: ₹%.2f\n",
               temp->orderId, temp->customerName, temp->tableNumber,
               temp->foodItem, temp->quantity, temp->totalCost);
        temp = temp->next;
    }
}

// Serve the first order
void serveOrder() {
    if (head == NULL) {
        printf("No orders to serve.\n");
        return;
    }

    struct Order* temp = head;
    printf("Serving Order ID: %d - %s (Table %d), %s x%d, ₹%.2f\n",
           temp->orderId, temp->customerName, temp->tableNumber,
           temp->foodItem, temp->quantity, temp->totalCost);
    head = head->next;
    free(temp);
}

// Main Menu
int main() {
    int choice, tableNum, quantity, foodIndex;
    char name[SIZE];

    while (1) {
        printf("\n--- Restaurant Management ---\n");
        printf("1. Show Menu\n");
        printf("2. Add Order\n");
        printf("3. Show All Orders\n");
        printf("4. Serve Next Order\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                showMenu();
                break;
            case 2:
                printf("Enter customer name: ");
                fgets(name, SIZE, stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter table number: ");
                scanf("%d", &tableNum);

                showMenu();
                printf("Select food item number: ");
                scanf("%d", &foodIndex);
                if (foodIndex < 1 || foodIndex > menuSize) {
                    printf("Invalid menu choice.\n");
                    break;
                }

                printf("Enter quantity: ");
                scanf("%d", &quantity);

                addOrder(name, tableNum, menu[foodIndex - 1].name, quantity);
                break;

            case 3:
                showOrders();
                break;

            case 4:
                serveOrder();
                break;

            case 5:
                printf("Thank you! Exiting...\n");
                return 0;

            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}
