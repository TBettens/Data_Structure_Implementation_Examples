## Data Structure Implementation Examples

1. **Sequence Containers**
    1. Vector Implementation Examples
        1. Fixed Capacity
        2. Extendable Capacity
        3. Pointers as Contiguous (was Random-Access) Iterators
        4. Deep vs Shallow copies (move semantics)
    2. Singly Linked List Implementation Examples
        1. Circular with 1 Sentential
        2. Null-Terminated
        3. Forward Iterators
        4. Deep vs Shallow copies (move semantics)
        5. Recursion Examples via an Extended Interface
    3. Doubly Linked List Implementation Examples
        1. Circular with 1 Sentential
        2. Null-Terminated
        3. Bi-Directional Iterators
        4. Deep vs Shallow copies (move semantics)
2. **Container Adapters**
    1. Stack Implementation Examples
        1. Over Vector-like containers
        2. Over Array-like Containers
        3. Over List-like containers
        4. Deep vs Shallow copies (move semantics)
    2. Queue Implementation Examples
        1. Over Vector-like containers
        2. Over Array-like Containers
        3. Over List-like containers
        4. Deep vs Shallow copies (move semantics)
3. **Ordered Associative Containers**
    1. Binary Search Tree Implementation Examples
    2. AVL Tree Implementation Examples
        1. Bi-Directional Iterators
        4. Deep vs Shallow copies (move semantics)
        5. Recursion Examples via an Extended Interface
4. **Student**
    1. class Student is used as the kind of object to store in the above Data Structures
        1. Copy and Move Constructors
        2. Copy and Move Assignment
        3. Instance Attributes
        4. Class Attributes
        5. Three-way Comparison
        6. Life-Span Tracing


#### Notes:
1.  This repository uses symbolic links.  While these links are created when cloning, they are not (as far as I can determine), created if you download a zipped copy of the repository
2.  Windows 10+ users requires a couple extra settings so symbolic links get created and used correctly
    1. **Set git config** (*%USERPROFILE%\\.gitconfig*) to use symlinks globally with
       1. `git config --global core.symlinks true`  
    **or** explicitly when cloning with
       2. `git clone -c core.symlinks=true <URL>`<br/><br/>
    I've also set git config (<repository\.git\config)) to use symlinks locally with `git config --local core.symlinks true` for portability hopefully making the above "stick" 
       See [Add symlink file as file using Git on Windows](https://newbedev.com/add-symlink-file-as-file-using-git-on-windows)
    2. **Enable Developer Mode** or more narrowly, enable *mklink* for non-administrators  
       In Linux/Unix, the command to create a symbolic link is called `ln`. The user doesn't need any special privileges to use it. In Windows the equivalent is called `mklink`. It can be issued from the cmd prompt but the user (normally) needs to be admin with elevated privileges. If you have Windows 10 (or better), and turn on Developer Mode you'll be able to create Symbolic Links without requiring admin privileges. See [Enable your device for development](https://docs.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development) and [How to Turn On / Off Developer Mode in Windows 10](https://www.top-password.com/blog/turn-on-off-developer-mode-in-windows-10/#:~:text=Windows%2010%20how%20to%20Turn%20on%20Developer%20Mode,takes%20effect%20immediately%20without%20a%20restart.%20See%20More)
