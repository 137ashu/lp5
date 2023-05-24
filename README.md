# BFS DFS

#include<bits/stdc++.h>
using namespace std;
#include <omp.h>
 
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
 
void bfs(TreeNode* root) {
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        cout << node->val << " ";
        if (node->left) {
            q.push(node->left);
        }
        if (node->right) {
            q.push(node->right);
        }
    }
}

void dfs(TreeNode* root) {
    stack<TreeNode*> s;
    s.push(root);
    while (!s.empty()) {
        TreeNode* node = s.top();
        s.pop();
        cout << node->val << " ";
        if (node->right) {
            s.push(node->right);
        }
        if (node->left) {
            s.push(node->left);
        }
    }
}
 
 
void parallel_bfs(TreeNode* root) {
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int qSize = q.size();
        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            TreeNode* node;
            #pragma omp critical
            {
                node = q.front();
                q.pop();
                cout << node->val << " ";
            }
            if (node->left)  q.push(node->left);        
            if (node->right) q.push(node->right);
        }
    }
}
 
void parallel_dfs(TreeNode* root) {
    stack<TreeNode*> s;
    s.push(root);
    while (!s.empty()) {
        int sSize = s.size();
        #pragma omp parallel for
        for (int i = 0; i < sSize; i++) {
            TreeNode* node;
            #pragma omp critical
            {
                node = s.top();
                s.pop();
                cout << node->val << " ";
            }
            if (node->right) s.push(node->right);
            if (node->left) s.push(node->left);
        }
    }
}
 

void createTree(TreeNode*& root) {
    int val;
    std::cout << "Enter the value for the current node: ";
    std::cin >> val;
    root = new TreeNode(val);

    char choice;
    std::cout << "Do you want to add a left child to the node with value " << val << "? (y/n): ";
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        createTree(root->left);
    }

    std::cout << "Do you want to add a right child to the node with value " << val << "? (y/n): ";
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        createTree(root->right);
    }
}

 
int main() {
    TreeNode* root = nullptr;
    createTree(root);
 
    cout << "BFS traversal: ";
    bfs(root);
    cout << endl;
 
    cout << "Parallel BFS traversal: ";
    parallel_bfs(root);
    cout<<endl;
 
    cout << "DFS traversal: ";
    dfs(root);
    cout << endl;
 
    cout << "Parallel DFS traversal: ";
    parallel_dfs(root);
    return 0;
}

# CUDA (3)

#include <iostream>
#include <omp.h>
#include <climits>

using namespace std;

void min_reduction(int arr[], int n) {
  int min_value = INT_MAX;
  #pragma omp parallel for reduction(min: min_value)
  for (int i = 0; i < n; i++) {
    if (arr[i] < min_value) {
      min_value = arr[i];
    }
  }
  cout << "Minimum value: " << min_value << endl;
}

void max_reduction(int arr[], int n) {
  int max_value = INT_MIN;
  #pragma omp parallel for reduction(max: max_value)
  for (int i = 0; i < n; i++) {
    if (arr[i] > max_value) {
      max_value = arr[i];
    }
  }
  cout << "Maximum value: " << max_value << endl;
}

void sum_reduction(int arr[], int n) {
  int sum = 0;
  #pragma omp parallel for reduction(+: sum)
  for (int i = 0; i < n; i++) {
    sum += arr[i];
  }
  cout << "Sum: " << sum << endl;
}

void average_reduction(int arr[], int n) {
  int sum = 0;
  #pragma omp parallel for reduction(+: sum)
  for (int i = 0; i < n; i++) {
    sum += arr[i];
  }
  cout << "Average: " << (double)sum / n << endl;
}

int main() {
  int *arr, n;
  cout << "\nEnter total number of elements: ";
  cin >> n;
  arr = new int[n];
  cout << "\nEnter elements: ";
  for (int i = 0; i < n; i++) {
    cin >> arr[i];
  }

  min_reduction(arr, n);
  max_reduction(arr, n);
  sum_reduction(arr, n);
  average_reduction(arr, n);

  return 0;
}
  
# CUDA 4.1

%%cuda --name omp_cuda.cu
#include<stdio.h>
#include<cuda.h>

_global_ void arradd(int *x,int *y, int *z)    //kernel definition
{
  int id=blockIdx.x; 
/* blockIdx.x gives the respective block id which starts from 0 */
  z[id]=x[id]+y[id];
}

int main()
{
    int a[6];
    int b[6];
    int c[6];
    int *d,*e,*f;
    int i;
    printf("\n Enter six elements of first array\n");
    for(i=0;i<6;i++)
    {
        scanf("%d",&a[i]);
    }
    printf("\n Enter six elements of second array\n");
        for(i=0;i<6;i++)
        {
            scanf("%d",&b[i]);
        }

/* cudaMalloc() allocates memory from Global memory on GPU */
    cudaMalloc((void **)&d,6*sizeof(int));
    cudaMalloc((void **)&e,6*sizeof(int));
    cudaMalloc((void **)&f,6*sizeof(int));

/* cudaMemcpy() copies the contents from destination to source. Here destination is GPU(d,e) and source is CPU(a,b) */
 cudaMemcpy(d,a,6*sizeof(int),cudaMemcpyHostToDevice);   
 cudaMemcpy(e,b,6*sizeof(int),cudaMemcpyHostToDevice);
 
/* call to kernel. Here 6 is number of blocks, 1 is the number of threads per block and d,e,f are the arguments */ 
arradd<<<6,1>>>(d,e,f); 

/* Here we are copying content from GPU(Device) to CPU(Host) */
 cudaMemcpy(c,f,6*sizeof(int),cudaMemcpyDeviceToHost);
    
printf("\nSum of two arrays:\n ");
    for(i=0;i<6;i++)
    {
        printf("%d\t",c[i]);
    }

/* Free the memory allocated to pointers d,e,f */
    cudaFree(d);
    cudaFree(e);
    cudaFree(f);

    return 0;
}
                     
                     
 # CUDA 4.2

%%cuda --name omp_cuda.cu
#include<stdio.h>
#include<cuda.h>
#define row1 2 /* Number of rows of first matrix */
#define col1 3 /* Number of columns of first matrix */
#define row2 3 /* Number of rows of second matrix */
#define col2 2 /* Number of columns of second matrix */

_global_ void matproduct(int *l,int *m, int *n)
{
    int x=blockIdx.x;
    int y=blockIdx.y;
    int k;
  
n[col2*y+x]=0;
for(k=0;k<col1;k++)
   {
    n[col2*y+x]=n[col2*y+x]+l[col1*y+k]*m[col2*k+x];
   }
}

int main()
{
    int a[row1][col1];
    int b[row2][col2];
    int c[row1][col2];
    int *d,*e,*f;
    int i,j;

    printf("\n Enter elements of first matrix of size 2*3\n");
    for(i=0;i<row1;i++)
    {
        for(j=0;j<col1;j++)
            {
                scanf("%d",&a[i][j]);
            }
    }
    printf("\n Enter elements of second matrix of size 3*2\n");
        for(i=0;i<row2;i++)
        {
            for(j=0;j<col2;j++)
                {
                    scanf("%d",&b[i][j]);
                }
        }

    cudaMalloc((void **)&d,row1*col1*sizeof(int));
    cudaMalloc((void **)&e,row2*col2*sizeof(int));
    cudaMalloc((void **)&f,row1*col2*sizeof(int));

 cudaMemcpy(d,a,row1*col1*sizeof(int),cudaMemcpyHostToDevice);
 cudaMemcpy(e,b,row2*col2*sizeof(int),cudaMemcpyHostToDevice);

dim3 grid(col2,row1);
/* Here we are defining two dimensional Grid(collection of blocks) structure. Syntax is dim3 grid(no. of columns,no. of rows) */

    matproduct<<<grid,1>>>(d,e,f);

 cudaMemcpy(c,f,row1*col2*sizeof(int),cudaMemcpyDeviceToHost);
    printf("\nProduct of two matrices:\n ");
    for(i=0;i<row1;i++)
    {
        for(j=0;j<col2;j++)
        {
              printf("%d\t",c[i][j]);
        }
        printf("\n");
    }

    cudaFree(d);
    cudaFree(e);
    cudaFree(f);

    return 0;
}
  

#  Some other Github Links
  
Github Links

https://github.com/ABelekar2101/semVIII

https://github.com/Mit-byte/Sem-8-lab/tree/main

https://github.com/adhi22/LP5

https://github.com/Prago2001/LP-5

https://github.com/afnan47/sem8
