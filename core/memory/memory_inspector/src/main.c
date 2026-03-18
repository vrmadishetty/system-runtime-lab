#include <stdio.h>


int main()
{
    float a1 = 0, a2 = 0;
    float ans;
    char operator, option;

    
    printf("Welcome to the Calculator Program!\n");

    do {
        a1 = 0; a2 = 0;
        printf("Enter first number: ");
        if (scanf(" %f", &a1) != 1) {
            printf("Invalid input! Please enter numeric values only. \n");
            return 1;
        }

        printf("Enter second number: ");
        if (scanf(" %f", &a2) != 1) {
            printf("Invalid input! Please enter numeric values only. \n");
            return 1;
        }


        printf("Enter arithmetic operator (+, -, *, /, ^ (exponential)): ");
        scanf(" %c", &operator);

        switch (operator) 
        {
            case '+': 
                        ans = a1 + a2;
                        printf("%f %c %f = %f\n", a1, operator, a2, ans);
                        break;

            case '-': 
                        ans = a1 - a2;
                        printf("%f %c %f = %f\n", a1, operator, a2, ans);
                        break;
            
            case '*': 
                        ans = a1 * a2;
                        printf("%f %c %f = %f\n", a1, operator, a2, ans);
                        break;
           
            case '/': 
                        if(a2 == 0)
                        {
                            printf("Error: Division by zero is not allowed! \n");
                        }
                        else {
                            ans = a1 / a2;
                            printf("%f %c %f = %f\n", a1, operator, a2, ans);
                        }
                        
                        break;
            case '^':
                        if (a2 < 0){
                            printf("Negative powers are not supported in this version.\n");
                            break;
                        }
                        ans = 1;
                        for(int i =0; i < (int)a2; i++) {
                            ans *= a1;
                        }
                        printf("%f %c %f = %f\n", a1, operator, a2, ans);
                        break;

            default:
                    printf("operator selection is not valid! \n");
                    printf("Please enter the Correct operator such as +, -, *, /, %%");
                    break;
        }
        label:
            printf("Would you like to calculate again? (y/n): ");
            scanf(" %c", &option);
    } while (option == 'y' || option == 'Y');

    printf("Thank you for using the Calculator Program: Goodbye!\n");

    return 0;

}

