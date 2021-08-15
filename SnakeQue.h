typedef struct{
  int X;
  int Y;    
}Chain;

// Define the default capacity of a queue
#define SIZE 256
 
// A class to store a queue
class SnakeQue
{
  public:
    Chain *arr;     // array to store queue elements
    int capacity;   // maximum capacity of the queue
    int tail;       // tail points to the tail element in the queue (if any)
    int head;       // head points to the last element in the queue
    int count;      // current size of the queue
 

    SnakeQue(int size = SIZE);     // constructor
    ~SnakeQue();                   // destructor
 
    Chain *dequeue();
    void enqueue(Chain *x);
    Chain *peek();
    Chain *peek_ind(int id);
    void dump();
    int size();
    bool isEmpty();
    bool isFull();
};
 
// Constructor to initialize a queue
SnakeQue::SnakeQue(int size)
{ 
    arr = new Chain[size];
    capacity = size;
    tail = 0;
    head = -1;
    count = 0;
}
 
// Destructor to free memory allocated to the queue
SnakeQue::~SnakeQue() {
    delete[] arr;
}
 
// Utility function to dequeue the tail element
Chain *SnakeQue::dequeue()
{
    // check for queue underflow
    if (isEmpty())
    {
        Serial.println( "Underflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
 
    //Serial.println( "Removing " ); //<< arr[tail] << endl;
 
    tail = (tail + 1) % capacity;
    count--;
    return  &arr[tail];
}
 
// Utility function to add an item to the queue
void SnakeQue::enqueue(Chain *item)
{
    // check for queue overflow
    if (isFull())
    {
       Serial.println( "Overflow\nProgram Terminated\n");
        //exit(EXIT_FAILURE);
        return 0;
    }
 
   //Serial.println( "Inserting: ");
    //Serial.println(item);
    head = (head + 1) % capacity;
    memcpy( &arr[head], item, sizeof(Chain));
    count++;
}
 
// Utility function to return the head element of the queue
Chain *SnakeQue::peek()
{
    if (isEmpty())
    {
       Serial.println( "Underflow\nProgram Terminated\n");
    }
    return &arr[head];
}

Chain *SnakeQue::peek_ind(int id){
    return  &arr[(tail + id) % capacity];
}

 void SnakeQue::dump(){
  Serial.println("_______________");
  for(int i = 0; i < count; i++)
  {
     
    Serial.print( "X:");
    Serial.print( arr[tail + i].X);
    Serial.print( "Y:");
    Serial.println( arr[tail + i].Y);
  }
  
  }
 
// Utility function to return the size of the queue
int SnakeQue::size() {
    return count;
}
 
// Utility function to check if the queue is empty or not
bool SnakeQue::isEmpty() {
    return (size() == 0);
}
 
// Utility function to check if the queue is full or not
bool SnakeQue::isFull() {
    return (size() == capacity);
}
 
