#include <iostream> 
#include <vector>

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