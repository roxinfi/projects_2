#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>


#define HOMESCREEN "\033[2J"
#define GOTOHOME "\033[H"

void starters()
{
    printf("Starters\n");
    printf("1. Soup\n");
    printf("2. Salad\n");
    printf("3. Bread\n");
    printf("4. Cheese\n");
    printf("5. Fries\n");
    printf("6. Nachos\n");
    printf("7. Wings\n");
    printf("8. Onion Rings\n");
    printf("9. Mozzarella Sticks\n");
    printf("10. Jalapeno Poppers\n");
    printf("11. Potato Skins\n");
    printf("12. Chicken Tenders\n");
    printf("13. Shrimp Cocktail\n");
    printf("14. Calamari\n");
    printf("15. Bruschetta\n");
    printf("16. Hummus\n");
    printf("17. Guacamole\n");
    printf("18. Chips and Salsa\n");
    printf("19. Deviled Eggs\n");
    printf("20. Stuffed Mushrooms\n");
    printf("21. Spring Rolls\n");
    printf("22. Crab Cakes\n");
    printf("23. Escargot\n");
    printf("24. Caviar\n");
    printf("25. Oysters\n");
    printf("26. Clams\n");
    printf("27. Mussels\n");
    printf("28. Lobster Bisque\n");
    printf("29. Gazpacho\n");
    printf("30. Caesar Salad\n");
    printf("31. Garden Salad\n");
    printf("32. Greek Salad\n");
    printf("33. Caprese Salad\n");
    printf("34. Cobb Salad\n");
    printf("35. Spinach Salad\n");
    printf("36. Waldorf Salad\n");
    printf("37. Antipasto\n");
    printf("38. Tuna Salad\n");
    printf("39. Chicken Salad\n");
    printf("40. Egg Salad\n");
    printf("41. Macaroni Salad\n");
    printf("42. Potato Salad\n");
    printf("43. Coleslaw\n");
    printf("44. Pasta Salad\n");
    printf("45. Fruit Salad\n");
    printf("46. Ambrosia Salad\n");
    printf("47. Waldorf Salad\n");
    printf("48. Caesar Salad\n");
    printf("49. Greek Salad\n");
    printf("50. Caprese Salad\n");
    printf("51. Cobb Salad\n");
}

void maincourse()
{
    printf("Main Course\n");
    printf("1. Steak\n");
    printf("2. Chicken\n");
    printf("3. Pork\n");
    printf("4. Fish\n");
    printf("5. Lamb\n");
    printf("6. Duck\n");
    printf("7. Turkey\n");
    printf("8. Veal\n");
    printf("9. Rabbit\n");
    printf("10. Venison\n");
    printf("11. Beef Wellington\n");
    printf("12. Beef Stroganoff\n");
    printf("13. Beef Bourguignon\n");
    printf("14. Beef Stew\n");
    printf("15. Beef Brisket\n");
    printf("16. Beef Ribs\n");
    printf("17. Beef Roast\n");
    printf("18. Beef Tenderloin\n");
    printf("19. Beef Jerky\n");
    printf("20. Beef Tacos\n");
    printf("21. Beef Enchiladas\n");
    printf("22. Beef Fajitas\n");
    printf("23. Beef Quesadillas\n");
    printf("24. Beef Burritos\n");
    printf("25. Beef Nachos\n");
    printf("26. Beef Chili\n");
    printf("27. Beef Soup\n");
    printf("28. Beef Salad\n");
    printf("29. Beef Sandwich\n");
    printf("30. Beef Burger\n");
    printf("31. Beef Pizza\n");
    printf("32. Beef Pasta\n");
    printf("33. Beef Lasagna\n");
    printf("34. Beef Spaghetti\n");
    printf("35. Beef Macaroni\n");
    printf("36. Beef Ravioli\n");
    printf("37. Beef Tortellini\n");
    printf("38. Beef Gnocchi\n");
    printf("39. Beef Risotto\n");
    printf("40. Beef Paella\n");
    printf("41. Beef Sushi\n");
    printf("42. Beef Tempura\n");
    printf("43. Beef Teriyaki\n");
    printf("44. Beef Ramen\n");
    printf("45. Beef Udon\n");
    printf("46. Beef Soba\n");
    printf("47. Beef Pho\n");
}

void kidsmenu()
{
    printf("Kids Menu\n");
    printf("1. Chicken Nuggets\n");
    printf("2. Mac and Cheese\n");
    printf("3. Grilled Cheese\n");
    printf("4. PB&J\n");
    printf("5. Hot Dog\n");
    printf("6. Corn Dog\n");
    printf("7. Pizza\n");
    printf("8. Spaghetti\n");
    printf("9. Chicken Tenders\n");
    printf("10. French Fries\n");
    printf("11. Tater Tots\n");
    printf("12. Onion Rings\n");
    printf("13. Mozzarella Sticks\n");
    printf("14. Jalapeno Poppers\n");
    printf("15. Potato Skins\n");
    printf("16. Nachos\n");
    printf("17. Quesadilla\n");
    printf("18. Taquitos\n");
    printf("19. Mini Tacos\n");
    printf("20. Mini Burritos\n");
    printf("21. Mini Chimichangas\n");
    printf("22. Mini Enchiladas\n");
    printf("23. Mini Fajitas\n");
    printf("24. Mini Quesadillas\n");
    printf("25. Mini Nachos\n");
    printf("26. Mini Pizza\n");
    printf("27. Mini Spaghetti\n");
    printf("28. Mini Mac and Cheese\n");
    printf("29. Mini Grilled Cheese\n");
    printf("30. Mini PB&J\n");
    printf("31. Mini Hot Dog\n");
    printf("32. Mini Corn Dog\n");
    printf("33. Mini Chicken Nuggets\n");
    printf("34. Mini Chicken Tenders\n");
    printf("35. Mini French Fries\n");
    printf("36. Mini Tater Tots\n");
    printf("37. Mini Onion Rings\n");
}

void drinksnoalohol()
{
    printf("Drinks (non-alcoholic)\n");
    printf("1. Water\n");
    printf("2. Soda\n");
    printf("3. Juice\n");
    printf("4. Milk\n");
    printf("5. Tea\n");
    printf("6. Coffee\n");
    printf("7. Lemonade\n");
    printf("8. Iced Tea\n");
    printf("9. Iced Coffee\n");
    printf("10. Smoothie\n");
    printf("11. Milkshake\n");
    printf("12. Frappuccino\n");
    printf("13. Slushie\n");
    printf("14. Icee\n");
    printf("15. Float\n");
    printf("16. Egg Cream\n");
    printf("17. Italian Soda\n");
    printf("18. Shirley Temple\n");
    printf("19. Roy Rogers\n");
    printf("20. Arnold Palmer\n");
    printf("21. Virgin Mary\n");
    printf("22. Virgin Pina Colada\n");
    printf("23. Virgin Daiquiri\n");
    printf("24. Virgin Margarita\n");
    printf("25. Virgin Mojito\n");
    printf("26. Virgin Sangria\n");
    printf("27. Virgin Mimosa\n");
    printf("28. Virgin Bellini\n");
    printf("29. Virgin Cosmopolitan\n");
    printf("30. Virgin Martini\n");
    printf("31. Virgin Manhattan\n");
}

void drinksalcoholic()
{
    printf("Drinks (alcoholic)\n");
    printf("1. Beer\n");
    printf("2. Wine\n");
    printf("3. Whiskey\n");
    printf("4. Vodka\n");
    printf("5. Rum\n");
    printf("6. Tequila\n");
    printf("7. Gin\n");
    printf("8. Brandy\n");
    printf("9. Cognac\n");
    printf("10. Liqueur\n");
    printf("11. Absinthe\n");
    printf("12. Aquavit\n");
    printf("13. Baijiu\n");
    printf("14. Bourbon\n");
    printf("15. Cachaca\n");
    printf("16. Grappa\n");
    printf("17. Mezcal\n");
    printf("18. Pisco\n");
    printf("19. Rakia\n");
    printf("20. Shochu\n");
}

void deserts()
{
    char desert_item_numbers[] =  0;
    printf("Desserts\n");
    printf("1. Cake\n");
    printf("2. Pie\n");
    printf("3. Ice Cream\n");
    printf("4. Cookies\n");
    printf("5. Brownies\n");
    printf("6. Pudding\n");
    printf("7. Jello\n");
    printf("8. Parfait\n");
    printf("9. Sorbet\n");
    printf("10. Gelato\n");
    printf("11. Tiramisu\n");
    printf("12. Cheesecake\n");
    printf("13. Mousse\n");
    printf("14. Trifle\n");
    printf("15. Flan\n");
    printf("16. Creme Brulee\n");
    printf("17. Baklava\n");
    printf("18. Cannoli\n");
    printf("19. Macarons\n");
    printf("20. Eclairs\n");
    printf("21. Profiteroles\n");
    printf("22. Croquembouche\n");
    printf("23. Crepes\n");
    printf("24. Pancakes\n");
    printf("25. Waffles\n");
    printf("26. French Toast\n");
    printf("27. Donuts\n");
    printf("28. Churros\n");
    printf("29. Beignets\n");
    printf("30. Funnel Cake\n");
    printf("31. Cotton Candy\n");
}

void menu()
{
    printf(HOMESCREEN);
    printf(GOTOHOME);
    printf("Welcome to the restaurant!\n\r");
    printf("Please select a category:\n\r");
    printf("1. Starters\n\r");
    printf("2. Main Course\n\r");
    printf("3. Kids Menu\n\r");
    printf("4. Drinks (non-alcoholic) \n\r");
    printf("5. Drinks (alcoholic) \n\r");
    printf("6. Desserts\n\r");
}
void main()
{
    printf(HOMESCREEN);
    printf(GOTOHOME);
    menu();
    char user_input = 0;
    while(user_input != 'Q' && user_input != 'q')
    {
        scanf(" %c", &user_input);
        switch(user_input)
        {
            case '1':
                starters();
                break;
            case '2':
                maincourse();
                break;
            case '3':
                kidsmenu();
                break;
            case '4':
                drinksnoalohol();
                break;
            case '5':
                drinksalcoholic();
                break;
            case '6':
                deserts();
                break;
            case 'Q':
            case 'q':
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid input. Please try again.\n");
                break;
        }
    }

}