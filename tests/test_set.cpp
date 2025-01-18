#include "test_set.hpp"

using std::string;

static const num_type kEps = 0.000001;

AssertionResult test_scalar_prod(TestFunctionInputExtended input){
    size_t length = 0;
    vector<num_type> a, b;
    
    num_type etalon;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
        length = generate_rand_integer_number(input.min_length, input.max_length);
        a.resize(length);
        b.resize(length);
    }
    if(input.algebra_object_version == AlgebraObjectVersion::kWrong){
        if(generate_rand_integer_number(input.min_length, input.max_length)){
            b.resize(length+1);
        }else{
            a.resize(length+1);
        }
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        etalon = 0.;
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(a.data(), length);
        generate_zero_array(b.data(), length);
        etalon = 0.;
        break;
    case AlgebraObjectVersion::kIdentity:
        {
            generate_zero_array(b.data(), length);
            size_t rand_pos = generate_rand_integer_number(0, length-1);
            b[rand_pos] = 1.0;
            generate_rand_array(a.data(), length, input.min_value, input.max_value);
            etalon = a[rand_pos];
        }
        break;
    case AlgebraObjectVersion::kGeneral:
    {
        num_type rand_val = generate_rand_integer_number(input.min_value, input.max_value);
        for(size_t i=0;i<length;++i){
            if(i%2){
                a[i] = rand_val;
                b[i] = -1;
            }else{
                a[i] = rand_val;
                b[i] = 1;
            }
        }
        if(length%2){
            etalon = rand_val;
        }else{
            etalon = 0.;
        }
    }
        break;
    case AlgebraObjectVersion::kWrong:
        switch (input.function_type)
        {
        case FunctionOptimizationType::kSimple:
            return assert::assert_throw(scalar_product_simple, Exception(ErrorType::kUnequalLengthError, ""), a, b, length);
        case FunctionOptimizationType::kSimpleStd:
            return assert::assert_throw(scalar_product_std, Exception(ErrorType::kUnequalLengthError, ""), a, b, length);
        default:
            return assert::assert_false(true);
        }
        break;
    default:
        return assert::assert_false(true);
        break;
    }
    num_type test_result;
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        test_result = scalar_product_simple(a, b, length);
        break;
    case FunctionOptimizationType::kSimpleStd:
        test_result = scalar_product_std(a, b, length);
        break;
    case FunctionOptimizationType::kUnsafe:
        test_result = scalar_product_opt_unsafe(a, b, length);
        break;
    default:
        return assert::assert_false(true);
        break;
    }
    return assert::assert_eq(etalon, test_result);
}

AssertionResult test_matrix_prod(TestFunctionInputExtended input){
    size_t a_row_number = 0;
    size_t a_column_number = 0;
    size_t b_column_number = 0;
    vector<num_type> a, b, c;
    vector<num_type> etalon;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
        a_row_number = generate_rand_integer_number(input.min_length, input.max_length);
        a_column_number = generate_rand_integer_number(input.min_length, input.max_length);
        b_column_number = generate_rand_integer_number(input.min_length, input.max_length);
        a.resize(a_row_number*a_column_number);
        b.resize(a_column_number*b_column_number);
        c.resize(a_row_number*b_column_number);
        etalon.resize(a_row_number*b_column_number);

        generate_zero_array(c.data(), a_row_number*b_column_number);
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(a.data(), a_row_number*a_column_number);
        generate_zero_array(b.data(), a_column_number*b_column_number);
        break;
    case AlgebraObjectVersion::kIdentity:
        {
            generate_identity_matrix(a.data(), a_row_number, a_column_number);
            generate_rand_array(b.data(), a_column_number*b_column_number, input.min_value, input.max_value);
            generate_zero_array(etalon.data(), a_row_number*b_column_number);
            size_t max_i = std::min(a_row_number, a_column_number); // number of rows in a and in b
            for(size_t i=0;i<max_i;++i){
                for(size_t j=0;j<b_column_number;++j){
                    etalon[i*b_column_number+j] = a[i*a_column_number+i]*b[i*b_column_number+j];
                }
            }
        }
        break;
    case AlgebraObjectVersion::kGeneral:
        a_row_number = 3;
        a_column_number = 2;
        b_column_number = 4;
        etalon={    
            5, -1, 4, -1, 
            -1, 2, 1, 2, 
            3, 0, 3, 0
        };
        a = {1, 2, 1, -1, 1, 1};
        b = {1, 1, 2, 1, 2, -1, 1, -1};
        c.resize(a_row_number*b_column_number);
        break;
    case AlgebraObjectVersion::kWrong:
        {
            std::function<void(const std::vector<double> &, const std::vector<double> &, std::vector<double> &, size_t, size_t, size_t)> foo;
            switch (input.function_type)
            {
            case FunctionOptimizationType::kSimple:
                foo = matrix_prod_base_simple;
                break;
            default:
                return assert::assert_false(true);
                break;
            }
            a.resize(a_row_number*a_column_number+1);
            bool wrong_a = assert::assert_throw(foo, Exception(ErrorType::kUnequalLengthError, ""), a, b, c, a_row_number, a_column_number, b_column_number);
            a.resize(a_row_number*a_column_number);
            b.resize(a_column_number*b_column_number+1);
            bool wrong_b = assert::assert_throw(foo, Exception(ErrorType::kUnequalLengthError, ""), a, b, c, a_row_number, a_column_number, b_column_number);
            b.resize(a_column_number*b_column_number);
            c.resize(a_row_number*b_column_number+1);
            bool wrong_c = assert::assert_throw(foo, Exception(ErrorType::kUnequalLengthError, ""), a, b, c, a_row_number, a_column_number, b_column_number);
            return assert::assert_true(wrong_a&&wrong_b&&wrong_c);
        }
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported matrix version number for matrix product: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        matrix_prod_base_simple(a, b, c, a_row_number, a_column_number, b_column_number);
        break;
    default:
        return assert::assert_false(true);
        break;
    }
    return assert::assert_iterable_containers_eq(etalon, c, etalon.size());
}

AssertionResult test_qram_schmidt(TestFunctionInputExtended input){
    size_t vector_system_size = 0;
    size_t vector_length = 0;
    vector<vector<num_type>> vector_system;
    vector<vector<num_type>> orthogonal_system;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
        vector_system_size = generate_rand_integer_number(input.min_length, input.max_length);
        vector_length = generate_rand_integer_number(vector_system_size, input.max_length); // to prevent linear dependence of vectors
        vector_system.resize(vector_system_size);
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            vector_system[vec_index].resize(vector_length);
        }
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        break;
    case AlgebraObjectVersion::kZero:
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            generate_zero_array(vector_system[vec_index].data(), vector_length);
        }
        break;
    case AlgebraObjectVersion::kIdentity:
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            generate_zero_array(vector_system[vec_index].data(), vector_length);
            vector_system[vec_index][vec_index]=num_type(1.);
        }
        break;
    case AlgebraObjectVersion::kGeneral:
       for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            generate_rand_array(vector_system[vec_index].data(), vector_length, input.min_value, input.max_value);
        }
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported matrix version number for matrix product: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    switch (input.function_type)
    {
    case FunctionOptimizationType::kUnsafe:
        orthogonal_system = gram_schmidt_base_simple(vector_system);
        break;
    default:
        orthogonal_system.resize(vector_system_size);
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            vector_system[vec_index].resize(vector_length);
            generate_rand_array(vector_system[vec_index].data(), vector_length, input.min_value, input.max_value); // to make bad test result for not implemented functions
        }
        break;
    }
    if(input.algebra_object_version==AlgebraObjectVersion::kZero){ // the presence of zero vectors leads to nan in the proj calculation 
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            for(size_t next_vec_index = vec_index + 1; next_vec_index<vector_system_size;++next_vec_index){
                num_type res = scalar_product_opt_unsafe(orthogonal_system[vec_index], orthogonal_system[next_vec_index], vector_length);
                if(res == res){
                    return assert::assert_eq(vec_index, next_vec_index);  // to save indexes of wrong vectors
                }
            }
        }
    }else{
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            for(size_t next_vec_index = vec_index + 1; next_vec_index<vector_system_size;++next_vec_index){
                num_type res = scalar_product_opt_unsafe(orthogonal_system[vec_index], orthogonal_system[next_vec_index], vector_length);
                bool assert_result =  assert::assert_near(0., res, kEps);
                if(!assert_result){
                    return assert::assert_eq(vec_index, next_vec_index);  // to save indexes of wrong vectors
                }
            }
        }
    }
    return assert::assert_true(true);
}

AssertionResult test_vector_norm(TestFunctionInputExtended input){
    size_t length = 0;
    vector<num_type> vec;
    
    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
        length = generate_rand_integer_number(input.min_length, input.max_length);
        vec.resize(length);
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(vec.data(), length);
        break;
    case AlgebraObjectVersion::kIdentity:
        {
            generate_zero_array(vec.data(), length);
            size_t rand_pos = generate_rand_integer_number(0, length-1);
            vec[rand_pos] = 1.0;
        }
        break;
    case AlgebraObjectVersion::kGeneral:
        generate_rand_array(vec.data(), length, input.min_value, input.max_value);
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported matrix version number for matrix product: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    num_type test_norm = get_vector_norm(vec);
    num_type base_norm = 0.0;
    for(size_t i=0;i<length;++i){
        base_norm+=vec[i]*vec[i];
    }
    base_norm = sqrt(base_norm);
    return assert::assert_eq(base_norm, test_norm);
}

AssertionResult test_normalize_vector(TestFunctionInputExtended input){
    size_t length = 0;
    vector<num_type> vec;
    num_type etalon = 1.0;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
       length = generate_rand_integer_number(input.min_length, input.max_length);
       vec.resize(length);
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        etalon = 0.0;
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(vec.data(), length);
        etalon = 0.0;
        break;
    case AlgebraObjectVersion::kIdentity:
        {
            generate_zero_array(vec.data(), length);
            size_t rand_pos = generate_rand_integer_number(0, length-1);
            vec[rand_pos] = 1.0;
        }
        break;
    case AlgebraObjectVersion::kGeneral:
        generate_rand_array(vec.data(), length, input.min_value, input.max_value);
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported matrix version number for matrix product: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    num_type norm = get_vector_norm(vec);
    normalize_vector_inplace(vec, norm);
    return assert::assert_near(etalon, get_vector_norm(vec), kEps);
}

AssertionResult test_matrix_transposition(TestFunctionInputExtended input){
    size_t row_num = 0;
    size_t column_num = 0;
    vector<num_type> matrix;
    vector<num_type> T_matrix;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
       row_num = generate_rand_integer_number(input.min_length, input.max_length);
       column_num = generate_rand_integer_number(input.min_length, input.max_length);
       matrix.resize(row_num*column_num);
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(matrix.data(), row_num*column_num);
        break;
    case AlgebraObjectVersion::kIdentity:
        generate_identity_matrix(matrix.data(), row_num, column_num);
        break;
    case AlgebraObjectVersion::kGeneral:
        generate_rand_array(matrix.data(), row_num*column_num, input.min_value, input.max_value);
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported matrix version number for matrix product: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    T_matrix = transpose_matrix(matrix, row_num, column_num);
    bool result = true;
    for(size_t i=0;i<column_num;++i){
        for(size_t j=0;j<row_num;++j){
            result = result && (T_matrix[i*row_num+j] == matrix[j*column_num+i]);
            if(! result){
                return assert::assert_eq(i, j);
            }
        }
    }
    return assert::assert_true(result);
}

AssertionResult test_qr_decomposition(TestFunctionInputExtended input){
    size_t row_num = 0;
    size_t column_num = 0;

    vector<num_type> matrix;
    vector<num_type> Q_matrix;
    vector<num_type> R_matrix;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
        if(input.function_type == FunctionOptimizationType::kUnsafe){
            column_num = generate_rand_integer_number(input.min_length, input.max_length);
            row_num = generate_rand_integer_number(column_num, input.max_length);
            
        }else{
            row_num = generate_rand_integer_number(input.min_length, input.max_length);
            column_num = generate_rand_integer_number(input.min_length, input.max_length);
        }
        matrix.resize(row_num*column_num);
        Q_matrix.resize(row_num*column_num);
        R_matrix.resize(column_num*column_num);
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(matrix.data(), row_num*column_num);
        break;
    case AlgebraObjectVersion::kIdentity:
        generate_identity_matrix(matrix.data(), row_num, column_num);
        break;
    case AlgebraObjectVersion::kGeneral:
        generate_rand_array(matrix.data(), row_num*column_num, input.min_value, input.max_value);
        break;
    case AlgebraObjectVersion::kWrong:
        ///////////////////////////////////
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported matrix version number for matrix product: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    switch (input.function_type)
    {
    case FunctionOptimizationType::kUnsafe:
        QR_decomposition_base_simple(matrix, Q_matrix, R_matrix, row_num, column_num);
        break;
    default:
        return assert::assert_true(false);
    }
    vector<num_type> test_matrix(row_num*column_num);
    matrix_prod_base_simple(Q_matrix, R_matrix, test_matrix, row_num, column_num, column_num);
    return assert::assert_iterable_containers_near(matrix, test_matrix, kEps, row_num*column_num);
}