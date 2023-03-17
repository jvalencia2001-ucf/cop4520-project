#include <iostream> 
#include <vector>
#include <thread>

using namespace std;

vector<vector<int>> matrix_add(vector<vector<int>> matA, vector<vector<int>> matB) {
  
  vector<vector<int>> res(matA.size(), vector<int>(matA.size()));
  
  for (int i = 0; i < matA.size(); i++) {
    for (int j = 0; j < matA[0].size(); j++) {
      res[i][j] = matA[i][j] + matB[i][j];
    }
  }

  return res;
}

vector<vector<int>> matrix_sub(vector<vector<int>> matA, vector<vector<int>> matB) {
  
  vector<vector<int>> res(matA.size(), vector<int>(matA.size()));
  
  for (int i = 0; i < matA.size(); i++) {
    for (int j = 0; j < matA[0].size(); j++) {
      res[i][j] = matA[i][j] - matB[i][j];
    }
  }

  return res;
}

vector<vector<int>> strassen_matrix_mult(vector<vector<int>> A, vector<vector<int>> B) {
  int n = A.size();
  vector<vector<int>> C(n, vector<int>(n));

  if (n == 1) {
    C[0][0] = A[0][0] * B[0][0];
    return C;
  }

  int m = n / 2;

  vector<vector<int>> A11(m, vector<int>(m)), A12(m, vector<int>(m)), A21(m, vector<int>(m)), A22(m, vector<int>(m));
  vector<vector<int>> B11(m, vector<int>(m)), B12(m, vector<int>(m)), B21(m, vector<int>(m)), B22(m, vector<int>(m));

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      A11[i][j] = A[i][j];
      A12[i][j] = A[i][j + m];
      A21[i][j] = A[i + m][j];
      A22[i][j] = A[i + m][j + m];

      B11[i][j] = B[i][j];
      B12[i][j] = B[i][j + m];
      B21[i][j] = B[i + m][j];
      B22[i][j] = B[i + m][j + m];
    }
  }

  vector<vector<int>> M1 = strassen_matrix_mult(A11, matrix_sub(B12, B22));
  vector<vector<int>> M2 = strassen_matrix_mult(matrix_add(A11, A12), B22);
  vector<vector<int>> M3 = strassen_matrix_mult(matrix_add(A21, A22), B11);
  vector<vector<int>> M4 = strassen_matrix_mult(A22, matrix_sub(B21, B11));
  vector<vector<int>> M5 = strassen_matrix_mult(matrix_add(A11, A22), matrix_add(B11, B22));
  vector<vector<int>> M6 = strassen_matrix_mult(matrix_sub(A12, A22), matrix_add(B21, B22));
  vector<vector<int>> M7 = strassen_matrix_mult(matrix_sub(A11, A21), matrix_add(B11, B12));

  vector<vector<int>> C11 = matrix_add(matrix_sub(matrix_add(M5, M4), M2), M6);
  vector<vector<int>> C12 = matrix_add(M1, M2);
  vector<vector<int>> C21 = matrix_add(M3, M4);
  vector<vector<int>> C22 = matrix_sub(matrix_sub(matrix_add(M5, M1), M3), M7);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      C[i][j] = C11[i][j];
      C[i][j + m] = C12[i][j];
      C[i + m][j] = C21[i][j];
      C[i + m][j + m] = C22[i][j];
    }
  }

  return C;
}


vector<vector<int>> dot_matrix_mult(vector<vector<int>>& matA, vector<vector<int>>& matB) {
    int rowA = matA.size();
    int colA = matA[0].size();
    int rowB = matB.size();
    int colB = matB[0].size();

    cout << rowA << " " << colA << " " << rowB << " " << colB << endl; 
     vector<vector<int>> result(rowA, vector<int>(colB, 0));

    for(int i = 0; i < rowA; i++)
    {
        for(int j = 0; j < colB; j++)
        {
            for(int k = 0; k < colA; k++)
            {
                result[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }

    return result;
}

void mutlithread_matrix_mult_function(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, int row, int col, int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result += A[row][i] * B[i][col];
    }
    C[row][col] = result;
}

vector<vector<int>> multithread_matrix_mult(vector<vector<int>>& A, vector<vector<int>>& B) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    vector<vector<int>> C(m, vector<int>(p));

    vector<thread> threads;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            threads.emplace_back(mutlithread_matrix_mult_function, ref(A), ref(B), ref(C), i, j, n);
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    return C;
}


int main(){
    vector<vector<int>> mA = {{1, 2, 3, 4},{5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    vector<vector<int>> mB = {{17, 18, 19, 20},{21, 22, 23, 24}, {25, 26, 27, 28}, {29, 30, 31, 32}};
    vector<vector<int>> res = dot_matrix_mult(mA, mB);

    for(int i = 0; i < res.size(); i++)
    {
        for(int j = 0; j < res[0].size(); j++)
        {
            cout << res[i][j] << " ";
        }

        cout << endl;
    }

    return 0;

}