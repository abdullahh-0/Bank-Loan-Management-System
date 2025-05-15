#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define CUSTOMER_FILE "customers.txt"
#define LOAN_FILE "loans.txt"

void clearInputBuffer() 
{
    while (getchar() != '\n');
}
	
void header(const char* title) 
{
    printf("\n================ %s ================\n", title);
}

void printSeparator() 
{
    printf("===============================================\n");
}

// Structure for customer details
typedef struct 
{
    int customerNumber;
    char name[50];
    char dateOfBirth[20];
    char cnic[20];
    char contactNo[20];
    char currentAddress[100];
    char email[50];
} Customer;

// Structure for loan details
typedef struct 
{
    int customerNumber;
    char purpose[50];
    float loanAmount;
    int repaymentMonths;
    float interestRate;
    float monthlyInstallment;
    float remainingAmount;
    int creditScore;
    char status[20];
} Loan;

// Validate integer input
int validatePositiveInt(const char* prompt) 
{
    char input[100];
    int value;
    char* endptr;

    do 
    {
        printf("%s: ", prompt);
        fgets(input, sizeof(input), stdin);
        strtok(input, "\n");
	
        value = strtol(input, &endptr, 10);

        if (*endptr != '\0' || endptr == input || value < 0) 
        {
            printf("Invalid input. Please enter a positive integer.\n");
            continue;
        } 
        else 
        {
            break;
        }
    } 
    while (1);

    return value;
}

// Validate float input
float validatePositiveFloat(const char* prompt) 
{
    char input[100];
    float value;
    char* endptr;

    do 
    {
        printf("%s: ", prompt);
        fgets(input, sizeof(input), stdin);
        strtok(input, "\n");

        value = strtof(input, &endptr);

        if (*endptr != '\0' || endptr == input || value < 0.0) 
        {
            printf("Invalid input. Please enter a positive number.\n");
            continue; 
        } 
        else 
        {
            break;
        }
    } 
    while (1);

    return value;
}

// Validate string input
void validateStringInput(const char* prompt, char* buffer, int size) 
{
    do 
    {
        printf("%s: ", prompt);
        fflush(stdin);
        fgets(buffer, size, stdin);
        strtok(buffer, "\n");

        if (strlen(buffer) == '\0') 
        {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        
        if (strspn(buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ") != strlen(buffer)) 
        {
            printf("Invalid input.\nOnly alphabets are allowed.\nPlease try again.\n");
            continue;
        } 
        else 
        {
            break;
        }
    } 
    while (1);
}

// Validate CNIC
void validateCNIC(char* buffer, int requiredLength) 
{   
	int length = 0;
    while (1) 
    {
        printf("Enter CNIC (exactly %d digits, no dashes): ", requiredLength);
        fflush(stdin);
        fgets(buffer, requiredLength + 2, stdin); 
        strtok(buffer, "\n");

        length = strlen(buffer);
				
        if ((length != requiredLength) || (strspn(buffer, "1234567890") != strlen(buffer))) 
        { 
            printf("Invalid input.\nCNIC must be exactly %d digits and can only contain digits.\nPlease try again.\n" , requiredLength);
            length = 0;
            continue;
        }
        else 
        {
            break; 
        }
    }
}

// Validate Contact Number
void validateContactNumber(char* buffer, int requiredLength) 
{
    while (1) 
    {
        printf("Enter Contact Number (exactly %d digits, no spaces): ", requiredLength);
        fgets(buffer, requiredLength + 2, stdin); 
        strtok(buffer, "\n"); 

        int length = strlen(buffer);

        if ((length != requiredLength) || (strspn(buffer, "1234567890") != strlen(buffer)))
        {
            printf("Invalid input.\nContact Number must be exactly %d digits and can only contain digits.\nPlease try again.\n" , requiredLength);
            continue;
        } 
        else 
        {
            break; 
        }
    }
}

// Validate DOB
void validateDOB(char* buffer, int minLength, int maxLength) 
{
    while (1) 
    {
        printf("Enter Date of Birth (format dd mm yyyy or ddmmyyyy): ");
        fflush(stdin);
        fgets(buffer, maxLength, stdin); 
        strtok(buffer, "\n"); 

        int length = strlen(buffer);

        if ((length < minLength || length > maxLength) || (strspn(buffer, "0123456789 ") != length)) 
        {
            printf("Invalid input. DOB must be %d-%d characters and contain only digits or spaces.\n", minLength, maxLength);
            continue;
        }
        
        if (strchr(buffer, ' ') != NULL) 
        {
            int day, month, year;
            if (sscanf(buffer, "%d %d %d", &day, &month, &year) != 3 || day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2024) 
            {
                printf("Invalid input. Please ensure the format is correct and values are valid.\n");
                continue;
            }
        }
        break;
    }
}

// Check for present CNIC in record/file
int checkCNICExists(const char* cnic) 
{
    FILE* file = fopen(CUSTOMER_FILE, "rb");
    if (!file) 
    {
        printf("No customer records found.\n");
        return 0; 
    }

    Customer customer;
    while (fread(&customer, sizeof(Customer), 1, file)) 
    {
        if (strcmp(customer.cnic, cnic) == 0) 
        {
            printf("\nA customer with this CNIC is already registered.\n");
            printf("Details of the registered customer:\n");
            printf("-------------------------\n");
            printf("Customer Number: %d\n", customer.customerNumber);
            printf("Name: %s\n", customer.name);
            printf("DOB: %s\n", customer.dateOfBirth);
            printf("Contact No: %s\n", customer.contactNo);
            printf("Email: %s\n", customer.email);
            printf("Address: %s\n", customer.currentAddress);
            printf("-------------------------\n");
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0;
}

// Customer management

// Adding a customer
void addCustomer() 
{
    Customer customer;
    FILE* file = fopen(CUSTOMER_FILE, "ab");
    if (!file) 
    {
        perror("Error opening file");
        return;
    }

    validateStringInput("Enter Name", customer.name, sizeof(customer.name));
    
    validateDOB(customer.dateOfBirth, 8, 11);
    
	validateCNIC(customer.cnic, 13);	
    do 
    {
        if (checkCNICExists(customer.cnic)) 
        {
            printf("Cannot register with this CNIC. Please use a different CNIC.\n");
            return;
        }
    } 
    while (0);
		
	validateContactNumber(customer.contactNo, 11);
    	
	validateStringInput("Enter Current Address", customer.currentAddress, sizeof(customer.currentAddress));
    	
	printf("Enter Email: ");
    fgets(customer.email, sizeof(customer.email), stdin);
    strtok(customer.email, "\n");

    customer.customerNumber = time(NULL) % 10000;
    fwrite(&customer, sizeof(Customer), 1, file);
    fclose(file);

    printf("Customer added successfully.\nYour customer number is: %d\n", customer.customerNumber);
}

// Displaying a customer
void displayCustomers() 
{
    FILE* file = fopen(CUSTOMER_FILE, "rb");
    Customer customer;

    if (!file) 
    {
        printf("No customers found.\n");
        return;
    }

    header("Customer Details");
    printf("%-22s | %-22s | %-22s | %-22s | %-22s | %-30s | %-22s\n", "Customer Number", "Name", "DOB", "CNIC", "Contact", "Email", "Address");

    while (fread(&customer, sizeof(Customer), 1, file)) 
    {
        printf("%-22d | %-22s | %-22s | %-22s | %-22s | %-30s | %-22s\n", 
		customer.customerNumber, 
		customer.name, 
		customer.dateOfBirth, 
		customer.cnic, 
		customer.contactNo, 
		customer.email, 
		customer.currentAddress);
    }
    
    fclose(file);
}

// Searching a customer
void searchCustomerByNumber()
{
    int customerNumber, found = 0;
    FILE* file = fopen(CUSTOMER_FILE, "rb");
    Customer customer;

    if (!file) 
    {
        printf("No customers found.\n");
        return;
    }

    customerNumber = validatePositiveInt("Enter Customer Number");

    while (fread(&customer, sizeof(Customer), 1, file)) 
    {
        if (customer.customerNumber == customerNumber) 
        {
            printf("Customer Found:\nName: %s\nCNIC: %s\nContact: %s\n", customer.name, customer.cnic, customer.contactNo);
            found = 1;
            break;
        }
    }
    if (!found) 
    {
        printf("Customer not found.\n");
    }
    fclose(file);
}

// Loan management

// Calculating Installments
void calculateInstallments(Loan* loan) 
{
    float monthlyRate = loan->interestRate / (12 * 100);
    
    int n = loan->repaymentMonths;
    loan->monthlyInstallment = (loan->loanAmount * monthlyRate * pow(1 + monthlyRate, n)) / (pow(1 + monthlyRate, n) - 1);
    loan->remainingAmount = loan->loanAmount;
}

// Checking for existing loan
int hasActiveLoan(int customerNumber) 
{
    FILE* loanFile = fopen(LOAN_FILE, "rb");
    if (!loanFile) 
    {
        printf("No loan records found.\n");
        return 0;
    }

    Loan loan;
    while (fread(&loan, sizeof(Loan), 1, loanFile)) 
    {
        if (loan.customerNumber == customerNumber && strcmp(loan.status, "Approved") == 0 && loan.remainingAmount > 0) 
        {
            fclose(loanFile);
            return 1;
        }
    }

    fclose(loanFile);
    return 0;
}

// Adding a loan
void applyForLoan() 
{
    Loan loan;
    FILE* loanFile = fopen(LOAN_FILE, "ab");
    if (!loanFile) 
    {
        perror("Error opening loan file");
        return;
    }

    loan.customerNumber = validatePositiveInt("Enter Customer Number");

    if (hasActiveLoan(loan.customerNumber)) 
    {
        printf("Customer already has an active loan. Loan application denied.\n");
        fclose(loanFile);
        return;
    }

    validateStringInput("Enter Loan Purpose", loan.purpose, sizeof(loan.purpose));
    
    loan.loanAmount = validatePositiveFloat("Enter Loan Amount (max 999,999)");

    while (loan.loanAmount > 999999) 
    {
        printf("Amount exceeds loan limit. Enter a value below 999,999.\n");
        loan.loanAmount = validatePositiveFloat("Enter Loan Amount (max 999,999)");
    }

    loan.repaymentMonths = validatePositiveInt("Enter Repayment Period (in months, max 60)");
    while (loan.repaymentMonths > 60) 
    {
        printf("Time period exceeds specified criteria.\n");
        loan.repaymentMonths = validatePositiveInt("Enter Repayment Period (in months, max 60)");
    }

    loan.interestRate = 5.0;

    loan.creditScore = validatePositiveInt("Enter Credit Score");
    if (loan.creditScore < 600) 
    {
        strcpy(loan.status, "Rejected");
        loan.monthlyInstallment = 0;
        loan.remainingAmount = 0;
        printf("Loan application rejected due to low credit score.\n");
    } 
    else 
    {
        calculateInstallments(&loan);
        strcpy(loan.status, "Approved");
        printf("Loan Approved.\nMonthly Installment: %.2f\n", loan.monthlyInstallment);
    }

    fwrite(&loan, sizeof(Loan), 1, loanFile);
    fclose(loanFile);
    printf("Loan application processed successfully.\n");
}

// Displaying loans
void displayLoans() 
{
    FILE* file = fopen(LOAN_FILE, "rb");
    Loan loan;

    if (!file) 
    {
        printf("No loans found.\n");
        return;
    }

    header("Loan Details");
    printf("%-20s | %-20s | %-20s | %-20s | %-20s | %-20s | %-20s\n", "Customer No.", "Purpose", "Amount", "Months", "Interest","Installment", "Status");

    while (fread(&loan, sizeof(Loan), 1, file)) 
    {
        printf("%-20d | %-20s | %-20.2f | %-20d | %-20.2f | %-20.2f | %-20s\n", loan.customerNumber, loan.purpose, loan.loanAmount, loan.repaymentMonths, loan.interestRate, loan.monthlyInstallment, loan.status);
    }
    fclose(file);
}

// Updating loan details
void updateLoanDetails() 
{
    FILE* loanFile = fopen(LOAN_FILE, "rb+");
    if (!loanFile) 
    {
        perror("Error opening loan file");
        return;
    }

    int customerNumber, found = 0;
    float paymentAmount;

    customerNumber = validatePositiveInt("Enter Customer Number to Update Loan Details");

    Loan loan;
    while (fread(&loan, sizeof(Loan), 1, loanFile)) 
    {
        if (loan.customerNumber == customerNumber) 
        {
            found = 1;
            printf("Current Loan Details:\n");
            printf("Loan Amount: %.2f\n", loan.loanAmount);
            printf("Remaining Amount: %.2f\n", loan.remainingAmount);

            do 
            {
                paymentAmount = validatePositiveFloat("Enter Payment Amount (0 to cancel)");
                
                if (paymentAmount == 0) 
                {
                    printf("Payment cancelled.\n");
                    break;
                }
                
                if (paymentAmount > loan.remainingAmount) 
                {
                    printf("Payment exceeds remaining amount. Please try again.\n");
                } 
                else 
                {
                	loan.remainingAmount -= paymentAmount;

                    fseek(loanFile, -sizeof(Loan), SEEK_CUR);
                    fwrite(&loan, sizeof(Loan), 1, loanFile);
                    printf("Payment recorded successfully.\nRemaining Amount: %.2f\n", loan.remainingAmount);

                    if (loan.remainingAmount == 0) 
                    {
                        printf("Loan fully repaid. Congratulations!\n");
                        strcpy(loan.status, "Completed");
                    }
                    break;
                }
            } 
            while (1);
            break;
        }
    }

    if (!found) 
    {
        printf("Loan not found for customer number %d.\n", customerNumber);
    }

    fclose(loanFile);
}

// Main Menu
void menu() 
{
    int choice;
    do 
	{
        header("Main Menu");
        printf("1. Add Customer\n");
        printf("2. Display All Customers\n");
        printf("3. Search Customer\n");
        printf("4. Apply for a Loan\n");
        printf("5. Display All Loans\n");
        printf("6. Update Loan Payment\n");
		printf("7. Exit\n");
        printSeparator();
        
        choice = validatePositiveInt("Enter your choice");
        
        if (choice < 1 || choice > 7)
        {
        	printf("Invalid choice! Try again.\n");
        	continue;
		}
		
		int confirm;
		
		printf("You selected option %d. Do you want to select another option? \nTo go back, enter 1 or any random number to continue. \nChoice: ", choice);
        scanf("%d", &confirm); 
        clearInputBuffer();

        if (confirm == 1) 
        {
            printf("Returning to the main menu...\n");
            continue;  
		}
        else
        {
        	printSeparator();
        	switch (choice) 
			{
            	case 1: 
				{
            		addCustomer();
					break;
				}
       	   		case 2: 
				{
    	        	displayCustomers();
					break;
				}
   	         	case 3: 
				{
       		     	searchCustomerByNumber();
					break;
				}
           		case 4: 
				{
            		applyForLoan();
					break;
				}
            	case 5: 
				{
        	   	 	displayLoans();
					break;
				}
         	  	case 6: 
				{
            		updateLoanDetails();
					break;
				}
				case 7: 
				{
            		header("Thank You for Using the System");
					break;
				}
            	default: 
				{
            		printf("Invalid choice. Please try again.\n");
				}
			}
		}
    } 
	while (choice != 7);
}

// Main function
int main() 
{
	system("COLOR 70");
	header("Bank Loan Managment System");
    
	menu();
    
	return 0;
}
