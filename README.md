---

# General Overview of the Program

The provided program works as a browser that can open and close tabs, each with its individual ID, and switch between them by going to the next or previous opened tab, or a specified one. The browser also supports opening one of the pages given as input in the input file named `"tema1.in"` with a specified ID, and accessing previously opened pages by going forward or backward. It also provides the possibility to print all the IDs of the currently opened tabs, circularly, starting from the current tab and going to the right, and to print the history of the accessed pages within the current tab, starting with the last forward page and going towards the first backward page, all the outputs being printed to the file `"tema1.out"`.

Sure! Here's the **usage explanation in Markdown format** that you can copy directly into your `README.md`:

---

## Usage

### 1. **Build the project**
To compile the project and generate the executable `browser`, run:

```bash
make build
```

This will compile all `.c` files into the `browser` executable.

### 2. **Run the program**
To execute the compiled program:

```bash
make run
```

This will run the `browser` executable (`./browser`).

### 3. **Package the project**
To create a zip file containing the `Makefile`, `README`, and all `.c` and `.h` files:

```bash
make pack
```

This will create a zip file named `311CA_MarasescuStefan_Tema1.zip`.

### 4. **Clean up**
To remove the compiled `browser` executable:

```bash
make clean
```

This will delete the `browser` executable from your project directory.

---

### Data Structures

The main data structures used to implement the browser are:

- **browser**: contains a pointer to the current tab and the circular doubly linked list of all the opened tabs.
- **tabList**: contains a pointer to the head of the list (sentinel) and the list's size.
- **tab_node**: contains a pointer to a tab and previous/next fields for double linking.
- **tab**: contains an ID and pointers to the current page and the backward/forward stacks, in order to have the history of the accessed pages within that tab.
- **stack**: has a pointer to the head of a singly linked list, which is also the current top of the stack. When inserting/deleting a new page in/from the stack, we do it at the start of the list.
- **stack_node**: has a pointer to a page and a next field for single linking.
- **page**: has an ID, a URL, and a dynamically allocated string for the description.

---

### Reading the Pages and the Commands

In function `main`, the input and the output file are opened and, after reading the number of pages, the function `read_pages` is called, which creates the array of pages. After that, the function `create_browser` dynamically allocates the browser, initializing the tab with the ID = 0, with its backward and forward stacks and the doubly linked list for the tabs. In order to read the commands from the input file, the function `read_commands` is called.

---

### Command Functions

- `new_tab` — increments the `last_id` variable and creates a new tab with the incremented ID, creates the forward and backward stacks for it and adds it to the doubly linked list (`tabList`) with the help of the function `dll_add_last`.

- `close_tab` — searches the current tab in the `tabList` by calling the function `search_current` so that it can remove it from the list by updating the links of the previous and next nodes, and free all the memory used by that tab.

- `open_tab` — reads the specified ID from the input and using a `while` loop it searches in the `tabList` for the tab with that ID. If that tab couldn't be found, the error message is printed, else it updates the current tab by copying all the fields from the found tab in the current tab pointer from the browser structure.

- `open_next_tab` — searches the current tab in the `tabList` by calling the function `search_current` and copies all the fields from the next node, which is after the found tab, in the current tab pointer from the browser structure.

- `open_previous_tab` — searches the current tab in the `tabList` by calling the function `search_current` and copies all the fields from the previous node, which is before the found tab, in the current tab pointer from the browser structure.

- `open_page` — reads the specified ID from the input and using a `for` loop it searches in the pages array for the page with that ID. If that page couldn't be found, the error message is printed, else it adds the old page to the backward stack, so that it can be accessed again at any time by using the `BACKWARD` command, and frees the forward stack.

- `open_backward_page` — checks if the stack is empty and prints an error message if it is. If it's not, it adds the current page to the forward stack, so that it can be accessed again at any time by using the `FORWARD` command and updates the current page with the backward page. After that, it calls the function `search_current` to search for the current page in the `tabList` in order to update it in the list as well.

- `open_forward_page` — checks if the stack is empty and prints an error message if it is. If it's not, it adds the current page to the backward stack, so that it can be accessed again at any time by using the `BACKWARD` command and updates the current page with the forward page. After that, it calls the function `search_current` to search for the current page in the `tabList` in order to update it in the list as well.

- `print` — searches the current tab in the `tabList` by calling the function `search_current` and uses a `while` loop to circularly iterate through the `tabList` (and skip the sentinel) to print to the output file the IDs of all the opened tabs in the browser.

- `print_history` — reads the specified ID from the input and using a `while` loop it searches in the `tabList` for the tab with that ID. If that tab couldn't be found, the error message is printed, else it first calls the function `print_list_reversed` on the forward stack, which prints the URLs from the pages found in the forward stack from bottom to top. After, it prints the URL from the currently opened page and calls the function `print_list` on the backward stack, which prints all the URLs from the pages found in the backward stack from top to bottom.

- ---
