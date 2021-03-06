
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//Maximum length of String for Run Length Encoding
#define MAX_RLEN 100


// calculating height of Huffman Tree
#define MAX_TREE_HT 100

int p[100];
char sym[100];  //Instant Variables For Shannon-Fano
int g_level=0;
/* ___________________________________________________CODE FOR HUFFMAN ____________________________________________________________*/
// A Huffman tree node
struct MinHeapNode {

	// One of the input characters
	char data;

	// Frequency of the character
	unsigned freq;

	// Left and right child of this node
	struct MinHeapNode *left, *right;
};

// A Min Heap: Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {

	// Current size of min heap
	unsigned size;

	// capacity of min heap
	unsigned capacity;

	// Array of minheap node pointers
	struct MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
	struct MinHeapNode* temp
		= (struct MinHeapNode*)malloc
(sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)

{

	struct MinHeap* minHeap
		= (struct MinHeap*)malloc(sizeof(struct MinHeap));

	// current size is 0
	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array
		= (struct MinHeapNode**)malloc(minHeap->
capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,
					struct MinHeapNode** b)

{

	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)

{

	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->
freq < minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->
freq < minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

// A utility function to check
// if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{

	return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0]
		= minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap,
				struct MinHeapNode* minHeapNode)

{

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)

{

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void printArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		printf("%d", arr[i]);

	printf("\n");
}

// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)

{

	return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)

{

	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)

{
	struct MinHeapNode *left, *right, *top;

	// Step 1: Create a min heap of capacity
	// equal to size. Initially, there are
	// modes equal to size.
	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

	// Iterate while size of heap doesn't become 1
	while (!isSizeOne(minHeap)) {

		// Step 2: Extract the two minimum
		// freq items from min heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		// Step 3: Create a new internal
		// node with frequency equal to the
		// sum of the two nodes frequencies.
		// Make the two extracted node as
		// left and right children of this new node.
		// Add this node to the min heap
		// '$' is a special value for internal nodes, not used
		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	// Step 4: The remaining node is the
	// root node and the tree is complete.
	return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top)

{

	// Assign 0 to left edge and recur
	if (root->left) {

		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	// Assign 1 to right edge and recur
	if (root->right) {

		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	// If this is a leaf node, then
	// it contains one of the input
	// characters, print the character
	// and its code from arr[]
	if (isLeaf(root)) {

		printf("%c: ", root->data);
		printArr(arr, top);
	}
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)

{
	// Construct Huffman Tree
	struct MinHeapNode* root
		= buildHuffmanTree(data, freq, size);

	// Print Huffman codes using
	// the Huffman tree built above
	int arr[MAX_TREE_HT], top = 0;

	printCodes(root, arr, top);
}

//Driver Function for Huffman
int Huffman_Driver()
{
    	int size ;
	printf("Enter the number of symbols :");
	scanf("%d",&size);
	char arr[size];
	int freq[size];
	for (int i=0;i<size;i++)
    {   arr[i]=(char)65+i;
        printf("Enter frequency of %c :",arr[i]);
        scanf("%d",&freq[i]);
        printf("\n");
    }

	HuffmanCodes(arr, freq, size);

	return 0;
}
/* ___________________________________________________CODE FOR Run Length ____________________________________________________________*/



/* Returns the Run Length Encoded string for the
   source string src */
char* encode(char* src)
{
    int rLen;
    char count[MAX_RLEN];
    int len = strlen(src);

    /* If all characters in the source string are different,
    then size of destination string would be twice of input string.
    For example if the src is "abcd", then dest would be "a1b1c1d1"
    For other inputs, size would be less than twice.  */
    char* dest = (char*)malloc(sizeof(char) * (len * 2 + 1));

    int i, j = 0, k;

    /* traverse the input string one by one */
    for (i = 0; i < len; i++) {

        /* Copy the first occurrence of the new character */
        dest[j++] = src[i];

        /* Count the number of occurrences of the new character */
        rLen = 1;
        while (i + 1 < len && src[i] == src[i + 1]) {
            rLen++;
            i++;
        }

        /* Store rLen in a character array count[] */
        sprintf(count, "%d", rLen);

        /* Copy the count[] to destination */
        for (k = 0; *(count + k); k++, j++) {
            dest[j] = count[k];
        }
    }

    /*terminate the destination string */
    dest[j] = '\0';
    return dest;
}
//Driver Function For Ren-Length
int RunLength_Driver()
{
    char str[MAX_RLEN] ;
    printf("Enter a String for Compression :" );
    scanf("%s",&str);
    char* res = encode(str);
    printf("\n\nThe Compressed String is : %s", res);
    getchar();

}

/*____________________________________________________________Code For Shannon-Fano _______________________________________*/

void shannon(int start,int end,char code[20][20],int level)
{        printf("d");
    int i=start;
    int j=end;
    int isum=p[i],jsum=p[j];

   if (level>g_level)
    {
        g_level=level;
    }
    while(i<(j-1))
    {
        while (isum>jsum && i<(j-1))
        {
            j--;
            jsum += p[j];
        }
        while (isum<jsum && i<(j-1))
        {
            i++;
            isum +=p[i];
        }
    }
    if (i==start)
        {
            code[start][level]='0';

        }
        else if ((i-start)>=1)
        {
            for (int k=start;k<=i;++k)
                code[k][level]='0';
            shannon(start,i,code,level+1);
        }
        if (j==end)
        {
            code[end][level]='1';
        }
        else if ((end-j)>=1)
        {
            for (int k=j;k<=end;++k)
                code[k][level]='1';
            shannon(j,end,code,level+1);
        }
}
//Driver Code For Shanon Fano
int Shannon_Driver()
{

    int n,i,j;
    printf("Enter the number of symbol:");
    scanf("%d",&n);
     char ch;

    //Take the symbols and sort them as user enters them using insertion sort
    for (i=0;i<n;i++)
    {
        int s;
        ch=(char)65+i;
        printf("Enter frequency of symbol %c : ",ch);
        scanf("%d",&s);

       p[i]=s;
       sym[i]=ch;
       }
       int temp;
       char t;
        for (i = 0; i < n; ++i)
        {
            for (j = i + 1; j < n; ++j)
            {
                if (p[i] < p[j])
                {
                    temp = p[i];
                    p[i] = p[j];
                    p[j] = temp;
                    t=sym[i];
                    sym[i]=sym[j];
                    sym[j]=t;
                }
            }
        }

    char code[20][20];

    for (i=0;i<n;++i)
    {
        for (j=0;j<n;j++)
        {
            code[i][j]='x';

        }
    }

    shannon(0,n-1,code,0);

        printf("\n\nSymbol  DATA   CODE\n");
        //Print the data and code
        for(i=0;i<n;i++)
        {

            printf("\n%c    %4d   :\t",sym[i],p[i]);
            for (j=0;j<=g_level;j++)
            {
                if((code[i][j] !='x'))
                    printf("%c",code[i][j]);
            }
        }

        return 0;
}


//DRIVER PROGRAM FOR THE ABOVE FUNCTIONS

int main ()
{

    int choice;



        printf("\n\nList Of  Data Compression Techniques: \n\n 1.Huffman Coding \n 2.Run Length Encoding \n 3.SHANNON FANO CODING\n\n ");
         printf("\nEnter Your Choice :\t");
         scanf("%d",&choice);

switch(choice)
{



case 1:
       printf("\n\n WELCOME TO HUFFMAN CODING :\n\n");
       Huffman_Driver();


       break;

case 2:
       printf("\n\n WELCOME TO RUN-LENGTH ENCODING:\n\n");
      RunLength_Driver();
      break;

case 3:
    printf("\n\n WELCOME TO SHANNON-FANO CODING:\n\n");
      Shannon_Driver();

       break;


break;


default :

        printf( "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxInvalid Inputxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n\n");

}












}
