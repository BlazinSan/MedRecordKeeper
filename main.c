#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_DEVICES 100
#define BUFFER_SIZE 256

typedef struct {
    char name[BUFFER_SIZE];
    char brand[BUFFER_SIZE];
    char model[BUFFER_SIZE];
    char serialNo[BUFFER_SIZE];
    double purchasePrice;
    char purchaseDate[BUFFER_SIZE];
    int warrantyPeriod;
    struct tm purchaseDateStruct;
    struct tm warrantyExpiryDate;
} MedicalDevice;

void inputDeviceDetails(MedicalDevice *device) {
    printf("Enter Device Name                   : ");
    fflush(stdin);
    fgets(device->name, BUFFER_SIZE, stdin);
    device->name[strcspn(device->name, "\n")] = 0;  // Remove newline character

    printf("Enter Brand                         : ");
    fgets(device->brand, BUFFER_SIZE, stdin);
    device->brand[strcspn(device->brand, "\n")] = 0;  // Remove newline character

    printf("Enter Model                         : ");
    fgets(device->model, BUFFER_SIZE, stdin);
    device->model[strcspn(device->model, "\n")] = 0;  // Remove newline character

    printf("Enter Serial Number                 : ");
    fgets(device->serialNo, BUFFER_SIZE, stdin);
    device->serialNo[strcspn(device->serialNo, "\n")] = 0;  // Remove newline character

    printf("Enter Purchase Price                : ");
    scanf("%lf", &device->purchasePrice);

    printf("Enter Purchase Date (DD-MM-YYYY)    : ");
    scanf("%s", device->purchaseDate);

    printf("Enter Warranty Period (in months)   : ");
    scanf("%d", &device->warrantyPeriod);

    // Parse the purchase date
    sscanf(device->purchaseDate, "%d-%d-%d", &device->purchaseDateStruct.tm_mday,
           &device->purchaseDateStruct.tm_mon, &device->purchaseDateStruct.tm_year);
    device->purchaseDateStruct.tm_year -= 1900;
    device->purchaseDateStruct.tm_mon -= 1;
    device->purchaseDateStruct.tm_isdst = -1;

    // Calculate The warranty expiry date
    device->warrantyExpiryDate = device->purchaseDateStruct;
    device->warrantyExpiryDate.tm_mon += device->warrantyPeriod;
    mktime(&device->warrantyExpiryDate);
}

void displayDeviceDetails(MedicalDevice *device) {
    printf("\n");
    printf("Device Name                          : %s\n", device->name);
    printf("Brand                                : %s\n", device->brand);
    printf("Model                                : %s\n", device->model);
    printf("Serial Number                        : %s\n", device->serialNo);
    printf("Purchase Price                       : %.2lf\n", device->purchasePrice);
    printf("Purchase Date                        : %s\n", device->purchaseDate);
    printf("Warranty Period (months)             : %d\n", device->warrantyPeriod);
    printf("\n");

    char warrantyExpiryDateStr[BUFFER_SIZE];
    strftime(warrantyExpiryDateStr, sizeof(warrantyExpiryDateStr), "%d-%m-%Y", &device->warrantyExpiryDate);
    printf("Warranty Expiry Date: %s\n", warrantyExpiryDateStr);

    time_t now = time(NULL);
    double remainingWarrantyDays = difftime(mktime(&device->warrantyExpiryDate), now) / (60 * 60 * 24);

    char purchaseDateStr[BUFFER_SIZE];
    strftime(purchaseDateStr, sizeof(purchaseDateStr), "%d-%m-%Y", &device->purchaseDateStruct);

    printf("Remaining Warranty Period: %.0f days from date of purchase (%s)\n", remainingWarrantyDays, purchaseDateStr);
}

void writeDeviceDetailsToFile(FILE *file, MedicalDevice *device) {
    fprintf(file, "Device Name: %s\n", device->name);
    fprintf(file, "Brand: %s\n", device->brand);
    fprintf(file, "Model: %s\n", device->model);
    fprintf(file, "Serial Number: %s\n", device->serialNo);
    fprintf(file, "Purchase Price: %.2lf\n", device->purchasePrice);
    fprintf(file, "Purchase Date: %s\n", device->purchaseDate);
    fprintf(file, "Warranty Period (months): %d\n", device->warrantyPeriod);

    char warrantyExpiryDateStr[BUFFER_SIZE];
    strftime(warrantyExpiryDateStr, sizeof(warrantyExpiryDateStr), "%d-%m-%Y", &device->warrantyExpiryDate);
    fprintf(file, "Warranty Expiry Date: %s\n", warrantyExpiryDateStr);

    time_t now = time(NULL);
    double remainingWarrantyDays = difftime(mktime(&device->warrantyExpiryDate), now) / (60 * 60 * 24);

    char purchaseDateStr[BUFFER_SIZE];
    strftime(purchaseDateStr, sizeof(purchaseDateStr), "%d-%m-%Y", &device->purchaseDateStruct);

    fprintf(file, "Remaining Warranty Period: %.0f days from date of purchase (%s)\n", remainingWarrantyDays, purchaseDateStr);
}

int main() {
    MedicalDevice devices[MAX_DEVICES];
    int totalDevices;
    FILE *file = fopen("medical_device_report.txt", "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    printf("Please key in the number of medical devices for registration: ");
    scanf("%d", &totalDevices);

    for (int i = 0; i < totalDevices; i++) {
        printf("\nEnter details for device %d:\n\n", i + 1);
        inputDeviceDetails(&devices[i]);
    }

    for (int i = 0; i < totalDevices; i++) {
        printf("\n--------------\n");
        printf("\Device %d:\n", i + 1);
        printf("--------------\n");
        displayDeviceDetails(&devices[i]);
        writeDeviceDetailsToFile(file, &devices[i]);
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}
