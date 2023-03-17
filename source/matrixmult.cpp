#include <iostream> 
#include <vector>
#include <thread>
#include <random>
#include <chrono>


using namespace std;

#define TEST_MATRIX_SIZE 4

int generate_random(int range_from, int range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);

    return distr(generator);
}

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

vector<vector<int>> dot_matrix_mult(vector<vector<int>>& matA, vector<vector<int>>& matB) {
    int rowA = matA.size();
    int colA = matA[0].size();
    int rowB = matB.size();
    int colB = matB[0].size();
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

void cputhread_matrix_mult_function(vector<vector<int>> A, vector<vector<int>> B, vector<vector<int>> &C, int rowStart, int rowEnd) {
  int n = A[0].size();

  for (int i = rowStart; i < rowEnd; i++) {
    for (int j = 0; j < B[0].size(); j++) {
      for (int k = 0; k < n; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

vector<vector<int>> cputhread_matrix_mult(vector<vector<int>> A,
                                          vector<vector<int>> B) {
  int m = A.size();
  int p = B[0].size();

  vector<vector<int>> C(m, vector<int>(p, 0));

  int numThreads = thread::hardware_concurrency();
  vector<thread> threads(numThreads);

  int chunkSize = m / numThreads;
  int rowStart = 0;

  for (int i = 0; i < numThreads; i++) {
    int rowEnd = (i == numThreads - 1) ? m : rowStart + chunkSize;
    threads[i] =
        thread(cputhread_matrix_mult_function, A, B, ref(C), rowStart, rowEnd);
    rowStart = rowEnd;
  }

  for (auto& t : threads) {
    t.join();
  }

  return C;
}

int main(){
    
    vector<vector<int>> mA(TEST_MATRIX_SIZE, vector<int>(TEST_MATRIX_SIZE));
    vector<vector<int>> mB(TEST_MATRIX_SIZE, vector<int>(TEST_MATRIX_SIZE));

    for(int i = 0; i < TEST_MATRIX_SIZE; i++) {
        for(int j = 0; j < TEST_MATRIX_SIZE; j++) {
            mA[i][j] = generate_random(1, 10);
            mB[i][j] = generate_random(1,10);
        }
    }

    auto start1 = std::chrono::steady_clock::now();  
    vector<vector<int>> res1 = dot_matrix_mult(mA, mB);
    auto finish1 = std::chrono::steady_clock::now();
    double elapsed_seconds1 = std::chrono::duration_cast<std::chrono::duration<double>>(finish1 - start1).count();

    auto start2 = std::chrono::steady_clock::now();  
    vector<vector<int>> res2 = strassen_matrix_mult(mA, mB);
    auto finish2 = std::chrono::steady_clock::now();
    double elapsed_seconds2 = std::chrono::duration_cast<std::chrono::duration<double>>(finish2 - start2).count();

    auto start3 = std::chrono::steady_clock::now();  
    vector<vector<int>> res3 = multithread_matrix_mult(mA, mB);
    auto finish3 = std::chrono::steady_clock::now();
    double elapsed_seconds3 = std::chrono::duration_cast<std::chrono::duration<double>>(finish3 - start3).count();

    auto start4 = std::chrono::steady_clock::now();  
    vector<vector<int>> res4 = cputhread_matrix_mult(mA, mB);
    auto finish4 = std::chrono::steady_clock::now();
    double elapsed_seconds4 = std::chrono::duration_cast<std::chrono::duration<double>>(finish4 - start4).count();

    cout << "Dot product method: " << elapsed_seconds1 << " seconds." << endl;
    for(int i = 0; i < res1.size(); i++)
    {
        for(int j = 0; j < res1[0].size(); j++)
        {
            cout << res1[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;

    cout << "Strassen method: " << elapsed_seconds2 << " seconds." << endl;
    for(int i = 0; i < res2.size(); i++)
    {
        for(int j = 0; j < res2[0].size(); j++)
        {
            cout << res2[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;

    cout << "Multithreaded method: " << elapsed_seconds3 << " seconds." << endl;
    for(int i = 0; i < res3.size(); i++)
    {
        for(int j = 0; j < res3[0].size(); j++)
        {
            cout << res3[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;

    cout << "CPUthreaded method: " << elapsed_seconds4 << " seconds." << endl;
    for(int i = 0; i < res4.size(); i++)
    {
        for(int j = 0; j < res4[0].size(); j++)
        {
            cout << res4[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;

    return 0;

}