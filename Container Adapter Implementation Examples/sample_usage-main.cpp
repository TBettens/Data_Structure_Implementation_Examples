import std;
import CSUF.CPSC131.DoublyLinkedList;
import CSUF.CPSC131.Stack;
import CSUF.CPSC131.Queue;
import CSUF.CPSC131.SinglyLinkedList;
import CSUF.CPSC131.Student;
import CSUF.CPSC131.Vector;




namespace
{
  // A simple demonstration to highlight the interfaces, similarities, and differences between Stacks and Queues.  Because Stacks and
  // Queues have (nearly) the same interface, the same demo driver is used for both.  The container tested is intentionally passed by
  // value (makes a local copy using the Stack's or Queue's copy constructor).


  // Stack and Queue Concepts
  template <typename C>
  concept is_Stack = requires( C c)
  {
    c.push( c.top() );
    c.pop();
    c.top();

    c.empty();
    c.size();
  };

  template <typename C>
  concept is_Queue = requires( C c)
  {
    c.push( c.front() );
    c.pop();
    c.front();
    c.back();

    c.empty();
    c.size();
  };

  template <typename C>
  concept Stack_or_Queue = is_Stack<C>  ||  is_Queue<C>;



  template<Stack_or_Queue Container_Type>
  void demo( Container_Type myContainer )
  {
    // std::type_info::name
    // Some implementations (such as MSVC, IBM, Oracle) produce a human-readable type name. Others, most notably gcc and clang,
    // return the mangled name, which is specified by the Itanium C++ ABI. The mangled name can be converted to human-readable form
    // using implementation-specific API such as abi::__cxa_demangle directly or through boost::core::demangle. It can also be piped
    // through the command-line utility c++filt -t.
    // https://en.cppreference.com/w/cpp/types/type_info/name.html
    std::print( std::cout, "\n\n\n"                         // Programming Note:  Two string literals separated only by white space are concatenated together by the pre-processor
                           "Using:     {}\n"                // some compilers generate more readable results with
                           "Function:  {}\n\n",             // type_info::name(), others with __func__.  So let's do them
                typeid( myContainer ).name(), __func__ );   // both just so we cover our bases




    // A stack is an Abstract Data Type, usually implemented as a limited interlace over some other data structure Things you can do
    // to a stack:
    myContainer.push( {"Tom"    } );
    myContainer.push( {"Aaron"  } );
    myContainer.push( {"Brenda" } );
    myContainer.pop();
    myContainer.push( {"Katelyn"} );


    // Display the contents.  Stacks and queues do not allow traversal (you can't see anything but the top (stack) or the front and
    // back (queue only), so to display the contents we have to inspect each element at the top and then remove it until the
    // container is empty.
    std::print( std::cout, "{}:  {:n:>}\n\n", ( is_Stack<Container_Type> ? "Stack" : "Queue" ), myContainer );

    for( ; !myContainer.empty(); myContainer.pop() )
    {
      constexpr auto format_string = " {:>30s}\n";
      if constexpr( is_Queue<Container_Type> )   std::print( std::cout, format_string, myContainer.front() );
      else       /* is_Stack<Container_Type> */  std::print( std::cout, format_string, myContainer.top()   );
    }
    std::println( std::cout );


  }
}     // namespace






int main()
{
  using CSUF::CPSC131::Student,
        CSUF::CPSC131::Queue,
        CSUF::CPSC131::Stack,
        CSUF::CPSC131::Vector,
        CSUF::CPSC131::SinglyLinkedList,
        CSUF::CPSC131::DoublyLinkedList;

  try
  {
    std::locale::global( std::locale( "en_US.UTF-8" ) );

    /*******************************************************************************************************************************
    **  STACKS
    *******************************************************************************************************************************/
    /////////////////// Stacks over lists //////////////////////
    { Stack<Student, SinglyLinkedList<Student>> myStack;    // empty stack where stack is implemented over a singly linked list
      demo( myStack );
    }

    { Stack<Student, DoublyLinkedList<Student>> myStack;    // empty stack where stack is implemented over a doubly linked list
      demo( myStack );
    }

    { Stack<Student, std::list<Student>> myStack;           // empty stack where stack is implemented over standard doubly linked list
      demo( myStack );
    }

    { Stack<Student, std::forward_list<Student>> myStack;   // empty stack where stack is implemented over standard singly linked list
      demo( myStack );
    }

    { Stack<Student, std::deque<Student>> myStack;          // empty stack where stack is implemented over standard deque (pronounced "deck"; double ended queue)
      demo( myStack );
    }



    /////////////////// Stacks over vectors //////////////////////
    { Stack<Student> myStack;                               // empty stack where stack is implemented over an extendable vector (the default)
      demo( myStack );
    }

    { Stack<Student, Vector<Student, CSUF::CPSC131::VectorPolicy::FIXED>> myStack;    // empty stack where stack is implemented over a fixed-capacity vector
      demo( myStack );
    }

    { Stack<Student, std::vector<Student>> myStack;         // empty stack where stack is implemented over standard vector
      demo( myStack );
    }



    /////////////////// Stacks over arrays //////////////////////
    { Stack<Student, std::array<Student, 10>> myStack;      // empty stack where stack is implemented over a fixed sized standard array
      demo( myStack );
    }

    //{ Stack<Student, Student[10]> myStack;                // empty stack where queue is implemented over a fixed sized native array
    //  demo( myStack );                                    // Not yet supported for Stack, see Queue for example
    //}



    /////////////////// STL Stack Examples //////////////////////
    { std::stack<Student> myStack;                          // default standard stack (uses std::deque as underlying container)
// #warning "gcc 15.1 does not yet support formatting the adapter containers but expected in 15.2"
// and MSC++ doesn't support #warning
      demo( myStack );
    }

    { std::stack<Student, std::list<Student>> myStack;      // standard stack with standard doubly linked list as underlying container
      demo( myStack );
    }

    { std::stack<Student, std::vector<Student>> myStack;    // standard stack with standard vector as underlying container
      demo( myStack );
    }






    /*******************************************************************************************************************************
    **  QUEUES
    *******************************************************************************************************************************/
    /////////////////// Queues over lists //////////////////////
    { Queue<Student> myQueue;                               // empty queue where queue is implemented over a singly linked list (the default)
      demo( myQueue);
    }

    { Queue<Student, DoublyLinkedList<Student>> myQueue;    // empty queue where queue is implemented over a doubly linked list
      demo( myQueue );
    }

    { Queue<Student, std::list<Student>> myQueue;           // empty queue where queue is implemented over standard doubly linked list
      demo( myQueue );
    }

    // cannot create a queue over a std::forward_list. It does not support pushing to the back in constant time

    { Queue<Student, std::deque<Student>> myQueue;           // empty queue where queue is implemented over standard deque (pronounced "deck"; double ended queue)
      demo( myQueue );
    }



    /////////////////// Queues over vectors //////////////////////
    { Queue<Student, Vector<Student>> myQueue;              // empty queue where queue is implemented over an extendable vector
      demo( myQueue );
    }

    { Queue<Student, Vector<Student, CSUF::CPSC131::VectorPolicy::FIXED>> myQueue;    // empty queue where queue is implemented over a fixed-capacity vector
      demo( myQueue );
    }

    { Queue<Student, std::vector<Student>> myQueue;         // empty queue where queue is implemented over standard vector
      demo( myQueue );
    }



    /////////////////// Queues over arrays //////////////////////
    {
      Queue<Student, std::array<Student, 10>> myQueue;      // empty queue where queue is implemented over a fixed sized standard array
      demo( myQueue );
    }

    { Queue<Student, Student[10]> myQueue;                  // empty queue where queue is implemented over a fixed sized native array
      demo( myQueue );
    }




    /////////////////// STL Queue Examples //////////////////////
    // Standard Queue usage with standard containers
    { std::queue<Student> myQueue_5;                        // default standard queue (uses std::deque as underlying container)
// #warning "gcc 15.1 does not yet support formatting the adapter containers but expected in 15.2"
// and MSC++ doesn't support #warning
      demo( myQueue_5 );
    }

    { std::queue<Student, std::list<Student>> myQueue_6;    // standard queue with standard doubly linked list as underlying container
      demo( myQueue_6 );
    }
  }

  catch (const std::exception & ex)
  {
    std::cerr << ex.what() << '\n';
  }
}    // main()











// For testing purposes, explicitly instantiate the class template.  Template class member functions are only instantiated, and thus
// semantically checked by the compiler, when used.  Explicitly instantiating the class forces all the member functions to be
// instantiated, and thus semantically checked by the compiler.  It enables the compiler to find errors in your code.
namespace CSUF::CPSC131
{
  // Stacks
  template class Stack< double                                                          >;

  template class Stack< Student,   std::array       <Student, 5                       > >;

  template class Stack< Student,   std::vector      <Student                          > >;
  template class Stack< Student,   Vector           <Student, VectorPolicy::FIXED     > >;
  template class Stack< Student,   Vector           <Student, VectorPolicy::EXTENDABLE> >;

  template class Stack< Student,   std::list        <Student                          > >;
  template class Stack< Student,   std::forward_list<Student                          > >;
  template class Stack< Student,   SinglyLinkedList <Student                          > >;
  template class Stack< Student,   DoublyLinkedList <Student                          > >;

  // Queues
  template class Queue_Over_Array < Student,   std::array       <Student, 5                       > >;

  template class Queue_Over_Vector< Student,   std::vector      <Student                          > >;
  template class Queue_Over_Vector< Student,   Vector           <Student, VectorPolicy::FIXED     > >;
  template class Queue_Over_Vector< Student,   Vector           <Student, VectorPolicy::EXTENDABLE> >;

  template class Queue_Over_List  < Student,   std::list        <Student                          > >;
  template class Queue_Over_List  < double,    SinglyLinkedList <double                           > >;
  template class Queue_Over_List  < Student,   SinglyLinkedList <Student                          > >;
  template class Queue_Over_List  < Student,   DoublyLinkedList <Student                          > >;

  // The standard singly linked list is not a viable option - it cannot push to the back
  // template class Queue< Student, std::forward_list<Student> >;
}    // namespace CSUF::CPSC131
