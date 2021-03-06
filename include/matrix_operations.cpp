#include <matrix_operations.hpp>

/// Method to read a csv file and return a Matrix object
Matrix MatrixOp::genfromtxt(std::string filename, char delim) {
    Matrix mat;
    std::ifstream file(filename);
    std::string line, cell;
    std::vector<std::string> cells;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        while (std::getline(ss, cell, delim)) {
            cells.push_back(cell);
        }
        mat.str_mat.push_back(cells);
        cells.clear();
    }

    return mat;
}

/// Method to initialize values of a Matrix object using a 2D vector
Matrix MatrixOp::init(std::vector<std::vector<double>> vec) {
    Matrix result;
    result.double_mat = vec;
    result.to_string();
    return result;
}

/// Method to initialize values of a Matrix object using a 2D vector
Matrix MatrixOp::init(std::vector<std::vector<std::string>> vec) {
    Matrix result;
    result.str_mat = vec;
    result.to_double();
    return result;
}

/// Method to initialize values of a Matrix object using a double
Matrix MatrixOp::init(double d) {
    Matrix result;
    std::vector<std::vector<double>> vec_d(1);
    vec_d[0].push_back(d);
    result.double_mat = vec_d;
    result.to_string();
    return result;
}

/// Method to initialize values of a Matrix object using a string
Matrix MatrixOp::init(std::string s) {
    Matrix result;
    std::vector<std::vector<std::string>> vec_s(1);
    vec_s[0].push_back(s);
    result.str_mat = vec_s;
    result.to_double();
    return result;
}

/// Method to initialize values of a Matrix object using a double vector
Matrix MatrixOp::init(std::vector<double> inner_d) {
    Matrix result;
    std::vector<std::vector<double>> vec_d(1, inner_d);
    result.double_mat = vec_d;
    result.to_string();
    return result;
}

/// Method to initialize values of a Matrix object using a string vector
Matrix MatrixOp::init(std::vector<std::string> inner_s) {
    Matrix result;
    std::vector<std::vector<std::string>> vec_s(1, inner_s);
    result.str_mat = vec_s;
    result.to_double();
    return result;
}

/// Method to concatenate/join two Matrix objects
Matrix MatrixOp::concatenate(Matrix mat1, Matrix mat2, std::string dim) {
    if (dim == "column") {
        if (mat1.row_length() != mat2.row_length())
            assert(("The Matrix objects should be of compatible dimensions", false));
        for (int i = 0; i < mat1.row_length(); i++) {
            mat1.str_mat[i].insert(mat1.str_mat[i].end(), mat2.str_mat[i].begin(),
                                   mat2.str_mat[i].end());
        }
    } else if (dim == "row") {
        if (mat1.col_length() != mat2.col_length())
            assert(("The Matrix objects should be of compatible dimensions", false));
        mat1.str_mat.insert(mat1.str_mat.end(), mat2.str_mat.begin(), mat2.str_mat.end());

    } else {
        assert(("Concatenate dimension wrong", false));
    }
    if (mat1.if_double) {
        mat1.to_double();
    }
    return mat1;
}

/// Method to calculate matrix multiplication
Matrix MatrixOp::matmul(Matrix mat1, Matrix mat2) {
    bool error = (mat1.if_double) && (mat2.if_double);
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (mat1.col_length() != mat2.row_length())
        assert(("The Matrix objects should be of compatible dimensions", false));

    Matrix mat;
    for (int i = 0; i < mat1.row_length(); i++) {
        std::vector<double> row;
        for (int j = 0; j < mat2.col_length(); j++) {
            row.push_back(0);
        }
        mat.double_mat.push_back(row);
    }
    for (unsigned long int i = 0; i < mat1.row_length(); i++) {
        for (unsigned long int j = 0; j < mat2.col_length(); j++) {
            for (unsigned long int k = 0; k < mat2.row_length(); k++) {
                mat.double_mat[i][j] += (mat1.double_mat[i][k] * mat2.double_mat[k][j]);
            }
        }
    }
    mat.to_string();
    return mat;
}

/// Method to create an Matrix of all elements 0
Matrix MatrixOp::zeros(int row, int col) {
    Matrix result;
    std::vector<std::string> vec(col, std::to_string(0));
    std::vector<std::vector<std::string>> mat(row, vec);
    result.str_mat = mat;
    result.to_double();
    return result;
}

/// Method to create an Matrix of all elements 1
Matrix MatrixOp::ones(int row, int col) {
    Matrix result;
    std::vector<std::string> vec(col, std::to_string(1));
    std::vector<std::vector<std::string>> mat(row, vec);
    result.str_mat = mat;
    result.to_double();
    return result;
}

/// Method to create an identity Matrix
Matrix MatrixOp::eye(int size) {
    Matrix result = zeros(size, size);
    for (int i = 0; i < size; i++) {
        result.str_mat[i][i] = "1";
        result.double_mat[i][i] = 1;
    }
    return result;
}

/// Method to calculate the Determinant of a Matrix
double MatrixOp::determinant(Matrix mat, int n) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (mat.row_length() != mat.col_length())
        assert(("The Matrix must be a square matrix", false));

    double D = 0;
    if (n == 1)
        return mat.double_mat[0][0];

    int sign = 1;
    Matrix temp = zeros(n, n);

    for (int f = 0; f < n; f++) {
        temp = cofactor(mat, temp, 0, f);
        D += sign * mat.double_mat[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }

    return D;
}

/// Method to calculate the Inverse of a Matrix
Matrix MatrixOp::inverse(Matrix mat) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (mat.row_length() != mat.col_length())
        assert(("The Matrix must be a square matrix", false));
    double det = determinant(mat, mat.col_length());
    if (det == 0)
        assert(("The Matrix is singular", false));

    Matrix adj = adjoint(mat);
    Matrix result = adj / det;
    return result;
}

/// Method to calculate the sum over an axis of a Matrix
Matrix MatrixOp::sum(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    Matrix result;
    if (dim == "column") {
        result = zeros(1, mat.col_length());
        for (int i = 0; i < mat.row_length(); i++) {
            for (int j = 0; j < mat.col_length(); j++) {
                result(0, j) = result(0, j) + mat(i, j);
                result(0, j);
            }
        }
    } else if (dim == "row") {
        result = zeros(mat.row_length(), 1);
        for (int i = 0; i < mat.col_length(); i++) {
            for (int j = 0; j < mat.row_length(); j++) {
                result(j, 0) = result(j, 0) + mat(j, i);
                result(j, 0);
            }
        }
    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
    return result;
}

/// Method to calculate the mean over an axis of a Matrix
Matrix MatrixOp::mean(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    Matrix result;
    if (dim == "column") {
        int n = mat.row_length();
        result = sum(mat, dim) / n;
    } else if (dim == "row") {
        int n = mat.col_length();
        result = sum(mat, dim) / n;
    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
    return result;
}

/// Method to calculate the standard deviation over an axis of a Matrix
Matrix MatrixOp::std(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    Matrix result;
    if (dim == "column") {
        int n = mat.row_length();
        Matrix mean = sum(mat, dim) / n;
        Matrix temp = mat - mean;
        result = sum((temp * temp), dim) / n;
    } else if (dim == "row") {
        int n = mat.col_length();
        Matrix mean = sum(mat, dim) / n;
        Matrix temp = mat - mean;
        result = sum((temp * temp), dim) / n;
    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
    return result;
}

/// Method to get the minimum value along an axis
Matrix MatrixOp::min(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (dim == "column") {
        std::vector<double> min_indices;
        for (int i = 0; i < mat.col_length(); i++) {
            std::vector<double> vec = mat.get_col(i);
            min_indices.push_back(*std::min_element(vec.begin(), vec.end()));
        }
        std::vector<std::vector<double>> res;
        res.push_back(min_indices);
        return matrix.init(res);

    } else if (dim == "row") {
        std::vector<double> temp;
        std::vector<std::vector<double>> min_indices;
        for (int i = 0; i < mat.row_length(); i++) {
            std::vector<double> vec = mat.get_row(i);
            temp.push_back(*std::min_element(vec.begin(), vec.end()));
            min_indices.push_back(temp);
            temp.clear();
        }
        return matrix.init(min_indices);

    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
}

/// Method to get the maximum value along an axis
Matrix MatrixOp::max(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (dim == "column") {
        std::vector<double> max_indices;
        for (int i = 0; i < mat.col_length(); i++) {
            std::vector<double> vec = mat.get_col(i);
            max_indices.push_back(*std::max_element(vec.begin(), vec.end()));
        }
        std::vector<std::vector<double>> res;
        res.push_back(max_indices);
        return matrix.init(res);

    } else if (dim == "row") {
        std::vector<double> temp;
        std::vector<std::vector<double>> max_indices;
        for (int i = 0; i < mat.row_length(); i++) {
            std::vector<double> vec = mat.get_row(i);
            temp.push_back(*std::max_element(vec.begin(), vec.end()));
            max_indices.push_back(temp);
            temp.clear();
        }
        return matrix.init(max_indices);

    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
}

/// Method to get the index of minimum value along an axis
Matrix MatrixOp::argmin(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (dim == "column") {
        std::vector<double> min_indices;
        for (int i = 0; i < mat.col_length(); i++) {
            std::vector<double> vec = mat.get_col(i);
            min_indices.push_back(std::min_element(vec.begin(), vec.end()) - vec.begin());
        }
        std::vector<std::vector<double>> res;
        res.push_back(min_indices);
        return matrix.init(res);

    } else if (dim == "row") {
        std::vector<double> temp;
        std::vector<std::vector<double>> min_indices;
        for (int i = 0; i < mat.row_length(); i++) {
            std::vector<double> vec = mat.get_row(i);
            temp.push_back(std::min_element(vec.begin(), vec.end()) - vec.begin());
            min_indices.push_back(temp);
            temp.clear();
        }
        return matrix.init(min_indices);

    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
}

/// Method to get the index of maximum value along an axis
Matrix MatrixOp::argmax(Matrix mat, std::string dim) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    if (dim == "column") {
        std::vector<double> max_indices;
        for (int i = 0; i < mat.col_length(); i++) {
            std::vector<double> vec = mat.get_col(i);
            max_indices.push_back(std::max_element(vec.begin(), vec.end()) - vec.begin());
        }
        std::vector<std::vector<double>> res;
        res.push_back(max_indices);
        return matrix.init(res);

    } else if (dim == "row") {
        std::vector<double> temp;
        std::vector<std::vector<double>> max_indices;
        for (int i = 0; i < mat.row_length(); i++) {
            std::vector<double> vec = mat.get_row(i);
            temp.push_back(std::max_element(vec.begin(), vec.end()) - vec.begin());
            max_indices.push_back(temp);
            temp.clear();
        }
        return matrix.init(max_indices);

    } else {
        assert(("Second parameter 'dimension' wrong", false));
    }
}

Matrix MatrixOp::sqrt(Matrix mat) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    std::vector<double> row;
    std::vector<std::vector<double>> res;
    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++)
            row.push_back(std::sqrt(mat.double_mat[i][j]));
        res.push_back(row);
        row.clear();
    }

    return matrix.init(res);
}

Matrix MatrixOp::power(Matrix mat1, Matrix mat2) {
    bool error1 = ((mat1.if_double) && (mat2.if_double));
    if (!error1)
        assert(("The Matrix objects should be first converted to double using to_double() method",
                error1));

    if ((mat1.row_length() == mat2.row_length()) && (mat1.col_length() == mat2.col_length())) {
        std::vector<double> row;
        std::vector<std::vector<double>> res;

        for (int i = 0; i < mat1.row_length(); i++) {
            for (int j = 0; j < mat1.col_length(); j++)
                row.push_back(std::pow(mat1.double_mat[i][j], mat2.double_mat[i][j]));
            res.push_back(row);
            row.clear();
        }
        return matrix.init(res);
    } else if ((mat1.row_length() == mat2.row_length()) && (mat2.col_length() == 1)) {
        std::vector<double> row;
        std::vector<std::vector<double>> res;

        for (int i = 0; i < mat1.row_length(); i++) {
            for (int j = 0; j < mat1.col_length(); j++)
                row.push_back(std::pow(mat1.double_mat[i][j], mat2.double_mat[i][0]));
            res.push_back(row);
            row.clear();
        }
        return matrix.init(res);
    } else if ((mat1.col_length() == mat2.col_length()) && (mat2.row_length() == 1)) {
        std::vector<double> row;
        std::vector<std::vector<double>> res;

        for (int i = 0; i < mat1.row_length(); i++) {
            for (int j = 0; j < mat1.col_length(); j++)
                row.push_back(std::pow(mat1.double_mat[i][j], mat2.double_mat[0][j]));
            res.push_back(row);
            row.clear();
        }
        return matrix.init(res);
    } else {
        assert(("The Matrix objects should be of compatible dimensions", false));
    }
}

Matrix MatrixOp::power(Matrix mat, double val) {
    bool error = mat.if_double;
    if (!error)
        assert(("The Matrix should be first converted to double using to_double() method", error));

    std::vector<double> row;
    std::vector<std::vector<double>> res;
    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++)
            row.push_back(std::pow(mat.double_mat[i][j], val));
        res.push_back(row);
        row.clear();
    }
    return matrix.init(res);
}

/** In Y, find list of indices of element whose value is val,
   then return the col'th column of the Matrix containing elements of those indices
*/
Matrix MatrixOp::slice_select(Matrix X, Matrix Y, double val, int col) {
    X = X.slice(0, X.row_length(), col, col + 1);

    bool is_compatible = (X.row_length() == Y.row_length());
    if (!is_compatible) {
        assert(("The Matrix objects should be of same dimensions", is_compatible));
    }
    std::vector<std::vector<double>> res;
    for (int i = 0; i < X.row_length(); i++) {
        if (Y.double_mat[i][0] == val) {
            res.push_back(X.get_row(i));
        }
    }
    return init(res);
}

/// Method to delete a row or column of a Matrix object
Matrix MatrixOp::delete_(Matrix mat, int index, std::string dim) {
    if (dim == "row") {
        Matrix sl1 = mat.slice(0, index, 0, mat.col_length());
        Matrix sl2 = mat.slice(index + 1, mat.row_length(), 0, mat.col_length());

        if (sl1.row_length() == 0)
            return sl2;
        if (sl2.row_length() == 0)
            return sl1;
        return concatenate(sl1, sl2, "row");
    } else if (dim == "column") {
        Matrix sl1 = mat.slice(0, mat.row_length(), 0, index);
        Matrix sl2 = mat.slice(0, mat.row_length(), index + 1, mat.col_length());

        if (sl1.col_length() == 0)
            return sl2;
        if (sl2.col_length() == 0)
            return sl1;
        return concatenate(sl1, sl2, "column");
    } else
        assert(("Second parameter 'dimension' wrong", false));
}

/// Method to calculate exponential of all elements in the Matrix object
Matrix MatrixOp::exp(Matrix mat) {
    bool error1 = mat.if_double;
    if (!error1)
        assert(("The Matrix should be first converted to double using to_double() method", error1));

    std::vector<double> row;
    std::vector<std::vector<double>> res;

    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++)
            row.push_back(std::exp(mat.double_mat[i][j]));
        res.push_back(row);
        row.clear();
    }

    return init(res);
}

/// Method to calculate natural logarithm of all elements in the Matrix object
Matrix MatrixOp::log(Matrix mat) {
    bool error1 = mat.if_double;
    if (!error1)
        assert(("The Matrix should be first converted to double using to_double() method", error1));

    std::vector<double> row;
    std::vector<std::vector<double>> res;

    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++)
            row.push_back(std::log(mat.double_mat[i][j]));
        res.push_back(row);
        row.clear();
    }

    return init(res);
}

/// Method to get absolute value of all elements in the Matrix object
Matrix MatrixOp::abs(Matrix mat) {
    bool error1 = mat.if_double;
    if (!error1)
        assert(("The Matrix should be first converted to double using to_double() method", error1));

    std::vector<double> row;
    std::vector<std::vector<double>> res;

    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++)
            row.push_back(std::abs(mat.double_mat[i][j]));
        res.push_back(row);
        row.clear();
    }

    return init(res);
}

/// Method to calculate reciprocal of all elements in the Matrix object
Matrix MatrixOp::reciprocal(Matrix mat) {
    bool error1 = mat.if_double;
    if (!error1)
        assert(("The Matrix should be first converted to double using to_double() method", error1));

    std::vector<double> row;
    std::vector<std::vector<double>> res;

    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++)
            row.push_back(1 / mat.double_mat[i][j]);
        res.push_back(row);
        row.clear();
    }

    return init(res);
}

// Helper methods

/// Helper method to calculate cofactor
Matrix MatrixOp::cofactor(Matrix mat, Matrix temp, int p, int q) {
    int i = 0, j = 0;
    for (int row = 0; row < mat.row_length(); row++) {
        for (int col = 0; col < mat.col_length(); col++) {
            if (row != p && col != q) {
                temp.double_mat[i][j++] = mat.double_mat[row][col];
                if (j == mat.col_length() - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
    temp.to_string();
    return temp;
}

/// Helper method to calculate the Adjoint of a Matrix
Matrix MatrixOp::adjoint(Matrix mat) {
    Matrix result = zeros(mat.row_length(), mat.col_length());

    if (mat.col_length() == 1) {
        result.double_mat[0][0] = 1;
        return result;
    }

    int sign = 1;
    Matrix temp = zeros(mat.row_length(), mat.col_length());

    for (int i = 0; i < mat.row_length(); i++) {
        for (int j = 0; j < mat.col_length(); j++) {
            temp = cofactor(mat, temp, i, j);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            result.double_mat[j][i] = (sign) * (determinant(temp, temp.col_length() - 1));
        }
    }

    return result;
}