#include <iostream>
#include <memory>
#include <assert.h>

// generic Matrix datatype, will be good later too
template <class T>
class Matrix {
private:
	// number of rows
	size_t dim_rows_;

	//number of columns
	size_t dim_columns_;

	// the data
	std::unique_ptr< std::unique_ptr<T[]>[] > matrix_{ nullptr };

public:
	// rw access to matrix elements
	T& operator()(size_t const &row, size_t const &col) {
		return matrix_[row][col];
	}

	// copy assignment operator
	Matrix<T>& operator=(Matrix<T> const &m) {
		size_t j;
		for (size_t i = 0; i < dim_rows_; ++i)
			matrix_[i].reset();
		matrix_.reset();
		dim_rows_ = m.dim_rows_;
		dim_columns_ = m.dim_columns_;
		matrix_ = std::make_unique< std::unique_ptr<T[]>[] >(dim_rows_);
		assert(matrix_ != nullptr);
		for (size_t i = 0; i < dim_rows_; ++i) {
			matrix_[i] = std::make_unique<T[]> (dim_columns_);
			assert(matrix_[i] != nullptr);
			for (j = 0; j < dim_columns_; ++j)
				matrix_[i][j] = m.matrix_[i][j];
		}
		return *this;
	}

	// move assigment operator
	Matrix<T>& operator=(Matrix<T> &&m) {
		std::swap(this->matrix_, m.matrix_);
		std::swap(this->dim_columns_, m.dim_columns_);
		std::swap(this->dim_rows_, m.dim_rows_);
		return *this;
	}

	// constructor without fill value
	Matrix(size_t const &a, size_t const &b) : dim_rows_(a), dim_columns_(b) {
		assert(a > 0 && b > 0);
		matrix_ = std::make_unique< std::unique_ptr<T[]>[] >(dim_rows_);
		assert(matrix_ != nullptr);
		for (size_t i = 0; i < dim_rows_; ++i) {
			matrix_[i] = std::make_unique<T[]> (dim_columns_);
			assert(matrix_[i] != nullptr);
		}
	}

	// constructor fills the matrix with fill_value
	Matrix(size_t const &a, size_t const &b, T const &fill_value) : Matrix(a, b) {
		size_t j;
		for (size_t i = 0; i < dim_rows_; ++i)
			for (j = 0; j < dim_columns_; ++j)
				matrix_[i][j] = fill_value;
	}

	// default constructor
	Matrix() : dim_rows_(0), dim_columns_(0) {}

	// move constructor
	Matrix(Matrix<T> &&m) {
		std::swap(this->matrix_, m.matrix_);
		std::swap(this->dim_columns_, m.dim_columns_);
		std::swap(this->dim_rows_, m.dim_rows_);
	}

	bool empty() const {
		return (matrix_ == nullptr);
	}

	// destructor
	~Matrix() = default;
};

int main(){
	size_t i;
	size_t j;
	size_t n{ 2 };
	
	Matrix<int> M{ n, n, 1 };
	std::cerr << "M is" << std::endl;
	for (j = 0; j<n; ++j){
		for (i = 0; i<n; ++i)
			std::cerr << M(i, j) << " ";
		std::cerr << std::endl;
	}
	
	Matrix<int> N{ n, n, 2 };
	
	M = N;
	
	std::cerr << "after M = N we get" << std::endl;
	for (j = 0; j<n; ++j){
		for (i = 0; i<n; ++i)
			std::cerr << M(i, j) << " ";
		std::cerr << std::endl;
	}
	
	M = Matrix<int>(n, n, 3);

	std::cerr << "after redefining it, M is" << std::endl;
	for (j = 0; j<n; ++j){
		for (i = 0; i<n; ++i)
			std::cerr << M(i, j) << " ";
		std::cerr << std::endl;
	}
	
	Matrix<int> X{ std::move(M) };
	std::cerr << "after X{ std::move(M) }" << std::endl;

	std::cerr << "X is empty == " << std::boolalpha << X.empty() << std::endl;

	std::cerr << "M is empty == " << std::boolalpha << M.empty() << std::endl;

	std::cerr << std::endl;

	std::cerr << "X is" << std::endl;

	for (j = 0; j<n; ++j){
		for (i = 0; i<n; ++i)
			std::cerr << X(i, j) << " ";
		std::cerr << std::endl;
	}

	return 0;
}
