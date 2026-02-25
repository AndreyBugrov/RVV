#include "test_set.hpp"

#include <iostream>

static const num_type kRelativeEps = 1E-7;

ExpectationResult test_dot_product(TestFunctionInputExtended input){
    size_t length = 0;
    vector<num_type> a, b;
    
    num_type etalon = 0.;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty && input.algebra_object_version != AlgebraObjectVersion::kIncorrect){
        length = generate_rand_length(input.min_length, input.max_length);
        a.resize(length);
        b.resize(length);
    }

    switch (input.algebra_object_version)
    {
    case AlgebraObjectVersion::kEmpty:
        break;
    case AlgebraObjectVersion::kZero:
        generate_zero_array(a.data(), length);
        generate_zero_array(b.data(), length);
        break;
    case AlgebraObjectVersion::kIdentity:
        {
            generate_zero_array(b.data(), length);
            size_t rand_pos = generate_rand_length(0, length-1);
            b[rand_pos] = 1.0;
            generate_rand_array(a.data(), length, input.min_value, input.max_value);
            etalon = a[rand_pos];
        }
        break;
    case AlgebraObjectVersion::kGeneral:
    {
        num_type rand_val = generate_rand_value(input.min_value, input.max_value);
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
    case AlgebraObjectVersion::kIncorrect:
        if(generate_rand_length(input.min_length, input.max_length)){
            b.resize(length + 1);
        }else{
            a.resize(length + 1);
        }
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Wrong AlgebraObjectVersion index: ", static_cast<int>(input.algebra_object_version)));
    }
    std::function<num_type (const vector_num&, const vector_num&, size_t)> foo;
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        foo = dot_product_simple;
        break;
    case FunctionOptimizationType::kStd:
        foo = dot_product_std;
        break;
    case FunctionOptimizationType::kUnsafe:
        foo = dot_product_simple_unsafe;
        break;
    case FunctionOptimizationType::kSimd:
        foo = dot_product_simd;
        break;
    case FunctionOptimizationType::kUnrolling:
        foo = dot_product_unrolling;
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Wrong FunctionOptimizationType index: ", static_cast<int>(input.function_type)));
    }
    if(input.algebra_object_version == AlgebraObjectVersion::kIncorrect){
        return expect::expect_throw(foo, Exception(ErrorType::kUnequalLengthError, ""), a, b, length);
    }
    num_type test_result = foo(a, b, length);
    return expect::expect_eq(etalon, test_result);
}

void block_matrix_filling(const TestFunctionInputExtended& input, size_t& a_row_count, size_t& a_column_count, size_t& b_column_count, vector<num_type>& a, vector<num_type>& b, vector<num_type>& c, vector<num_type>& etalon){
    a_row_count = generate_rand_length(1, 10) * kBlockSize;
    a_column_count = generate_rand_length(1, 10) * kBlockSize;
    b_column_count = generate_rand_length(1, 10) * kBlockSize;
    resize_and_generate_matrix(a, a_row_count, a_column_count, input.algebra_object_version, 0, input.max_value);
    resize_and_generate_matrix(b, a_column_count, b_column_count, input.algebra_object_version, 0, input.max_value);
    resize_and_generate_matrix(etalon, a_row_count, b_column_count);
    resize_and_generate_matrix(c, a_row_count, b_column_count);

    matrix_product_base_simple(a, b, etalon, a_row_count, a_column_count, b_column_count);
    std::function<void (const vector_num&, const vector_num&, vector_num&, size_t, size_t, size_t)> foo;
    switch (input.function_type)
    {
    case FunctionOptimizationType::kBlock:
        foo = matrix_product_row_block;
        break;
    case FunctionOptimizationType::kBlockScalar:
        foo = matrix_product_row_block_scalar;
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Wrong FunctionOptimizationType index: ", static_cast<int>(input.function_type)));
        break;
    }
    foo(a, b, c, a_row_count, a_column_count, b_column_count);
}

ExpectationResult test_matrix_product(TestFunctionInputExtended input){
    size_t a_row_count = 0;
    size_t a_column_count = 0;
    size_t b_column_count = 0;
    vector<num_type> a, b, c;
    vector<num_type> etalon;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty && input.algebra_object_version != AlgebraObjectVersion::kGeneral){
        a_row_count = generate_rand_length(input.min_length, input.max_length);
        a_column_count = generate_rand_length(input.min_length, input.max_length);
        b_column_count = generate_rand_length(input.min_length, input.max_length);
        resize_and_generate_matrix(a, a_row_count, a_column_count, input.algebra_object_version, input.min_value, input.max_value);
    }
    if(input.function_type == FunctionOptimizationType::kBlock || input.function_type == FunctionOptimizationType::kBlockScalar){
        block_matrix_filling(input, a_row_count, a_column_count, b_column_count, a, b, c, etalon);
        return expect::expect_indexable_containers_near(etalon, c, kRelativeEps, etalon.size(), true);
    }

    switch (input.algebra_object_version)
    {
        case AlgebraObjectVersion::kEmpty:
            break;
        case AlgebraObjectVersion::kIdentity:
            {
                resize_and_generate_matrix(b, a_column_count, b_column_count, AlgebraObjectVersion::kGeneral, input.min_value, input.max_value);
                resize_and_generate_matrix(etalon, a_row_count, b_column_count);
                size_t max_i = std::min(a_row_count, a_column_count); // number of rows in a and in b
                for(size_t i=0;i<max_i;++i){
                    for(size_t j=0;j<b_column_count;++j){
                        etalon[i*b_column_count+j] = a[i*a_column_count+i]*b[i*b_column_count+j];
                    }
                }
            }
            break;
        case AlgebraObjectVersion::kGeneral:
            a_row_count = 3;
            a_column_count = 2;
            b_column_count = 4;
            etalon={    
                5, -1, 4, -1, 
                -1, 2, 1, 2, 
                3, 0, 3, 0
            };
            a = {1, 2, 1, -1, 1, 1};
            b = {1, 1, 2, 1, 2, -1, 1, -1};

            break;
        default:
            resize_and_generate_matrix(b, a_column_count, b_column_count, input.algebra_object_version, input.min_value, input.max_value);
            break;
    }
    if(input.algebra_object_version != AlgebraObjectVersion::kIncorrect){
        resize_and_generate_matrix(c, a_row_count, b_column_count); // to create incorrect c matrix in incorrect lengths check 
    }
    
    std::function<void (const vector_num&, const vector_num&, vector_num&, size_t, size_t, size_t)> foo;
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        foo = matrix_product_base_simple;
        break;
    case FunctionOptimizationType::kRow:
        foo = matrix_product_row_simple;
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Wrong FunctionOptimizationType index: ", static_cast<int>(input.function_type)));
        break;
    }
    if(input.algebra_object_version == AlgebraObjectVersion::kIncorrect){
        resize_and_generate_matrix(c, a_row_count, b_column_count, AlgebraObjectVersion::kIncorrect);
        vector_num normal_a, normal_b, normal_c;
        resize_and_generate_matrix(normal_a, a_row_count, a_column_count);
        resize_and_generate_matrix(normal_b, a_column_count, b_column_count);
        resize_and_generate_matrix(normal_c, a_row_count, b_column_count);
        Exception required_exception = Exception(ErrorType::kUnequalLengthError, "");
        bool wrong_a, wrong_b, wrong_c;
        wrong_a = expect::expect_throw(foo, required_exception, a, normal_b, normal_c, a_row_count, a_column_count, b_column_count);
        wrong_b = expect::expect_throw(foo, required_exception, normal_a, b, normal_c, a_row_count, a_column_count, b_column_count);
        wrong_c = expect::expect_throw(foo, required_exception, normal_a, normal_b, c, a_row_count, a_column_count, b_column_count);
        return expect::expect_true(wrong_a && wrong_b && wrong_c);
    }
    foo(a, b, c, a_row_count, a_column_count, b_column_count);
    return expect::expect_indexable_containers_eq(etalon, c, etalon.size());
}

ExpectationResult test_gram_schmidt(TestFunctionInputExtended input){
    size_t vector_system_size = 0;
    size_t vector_length = 0;
    vector<vector<num_type>> vector_system;
    vector<vector<num_type>> orthogonal_system;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
        vector_system_size = generate_rand_length(input.min_length, input.max_length);
        if(input.algebra_object_version == AlgebraObjectVersion::kIncorrect){
            vector_length = generate_rand_length(input.min_length, vector_system_size);
        }else{
            vector_length = generate_rand_length(vector_system_size, input.max_length); // to prevent linear dependence of vectors
        }
        
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
    case AlgebraObjectVersion::kIncorrect:
        for(size_t vec_index = 0;vec_index<vector_system_size;++vec_index){
            generate_zero_array(vector_system[vec_index].data(), vector_length);
        }
        return expect::expect_throw(gram_schmidt_base_simple, Exception(ErrorType::kIncorrectLengthRatio, ""), vector_system);
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Unsupported AlgebraObjectVersion index: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        orthogonal_system = gram_schmidt_base_simple(vector_system);
        break;
    // case FunctionOptimizationType::kRow:
    //     gram_schmidt_matrix_simple;
    //     break;
    // case FunctionOptimizationType::kUnrolling:
    //     gram_schmidt_matrix_unrolling;
    //     break;
    // case FunctionOptimizationType::kSimd:
    //     gram_schmidt_matrix_simd;
    //     break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Unsupported FunctionOptimizationType index: ", static_cast<int>(input.function_type)));
    }
    if(input.algebra_object_version==AlgebraObjectVersion::kZero){ // the presence of zero vectors leads to nan in the proj calculation 
        for(size_t vec_index = 0; vec_index<vector_system_size; ++vec_index){
            for(size_t next_vec_index = vec_index + 1; next_vec_index<vector_system_size;++next_vec_index){
                num_type res = dot_product_simple_unsafe(orthogonal_system[vec_index], orthogonal_system[next_vec_index], vector_length);
                if(res == res){
                    return expect::expect_eq(vec_index, next_vec_index);  // to save indexes of wrong vectors
                }
            }
        }
    }else{
        size_t end_vec_index = vector_system_size - 1;
        if(input.algebra_object_version == AlgebraObjectVersion::kEmpty){
            end_vec_index = vector_system_size; // to protect from subtraction from zero
        }
        size_t checked_container_length = vector_system_size * (vector_system_size - 1) / 2;
        vector<num_type> dot_product_results(checked_container_length);
        vector<num_type> zero_container(checked_container_length);
        size_t offset = -vector_system_size;
        if(checked_container_length != 0){
            generate_zero_array(zero_container.data(), checked_container_length);
        }
        for(size_t vec_index = 0; vec_index < end_vec_index; ++vec_index){
            offset += (vector_system_size - vec_index);
            size_t dot_index = 0;
            for(size_t next_vec_index = vec_index + 1; next_vec_index < vector_system_size; ++next_vec_index, ++dot_index){
                num_type res = dot_product_simple_unsafe(orthogonal_system.at(vec_index), orthogonal_system.at(next_vec_index), vector_length);
                dot_product_results.at(offset + dot_index) = res;
            }
        }
        return expect::expect_indexable_containers_near(zero_container, dot_product_results, kRelativeEps, checked_container_length, true);
    }
    return expect::expect_true(true);
}

ExpectationResult test_vector_norm(TestFunctionInputExtended input){
    size_t length = 0;
    vector<num_type> vec;
    num_type etalon = 0.;
    
    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty && input.algebra_object_version != AlgebraObjectVersion::kGeneral){
        length = generate_rand_length(input.min_length, input.max_length);
    }
    if(input.algebra_object_version == AlgebraObjectVersion::kGeneral){
        length = 256;
    }
    vec.resize(length);

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
            size_t rand_pos = generate_rand_length(0, length-1);
            vec[rand_pos] = 1.0;
            etalon = 1.0;
        }
        break;
    case AlgebraObjectVersion::kGeneral:
        // memset does not work with non-zero floating-point values correctly (even memset(vec.data(), num_type(4), length * sizeof(num_type)) is incorrect)
        std::fill(vec.begin(), vec.end(), num_type(4));
        etalon = 64;
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Unsupported AlgebraObjectVersion index: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    num_type test_norm = get_vector_norm(vec);
    return expect::expect_eq(etalon, test_norm);
}

ExpectationResult test_normalize_vector(TestFunctionInputExtended input){
    size_t length = 0;
    vector<num_type> vec;
    num_type etalon = 1.0;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
       length = generate_rand_length(input.min_length, input.max_length);
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
            size_t rand_pos = generate_rand_length(0, length-1);
            vec[rand_pos] = 1.0;
        }
        break;
    case AlgebraObjectVersion::kGeneral:
        generate_rand_array(vec.data(), length, input.min_value, input.max_value);
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Unsupported AlgebraObjectVersion index: ", static_cast<int>(input.algebra_object_version)));
        break;
    }
    num_type norm = get_vector_norm(vec);
    normalize_vector_inplace(vec, norm);
    return expect::expect_near(etalon, get_vector_norm(vec), kRelativeEps);
}

ExpectationResult test_matrix_transposition(TestFunctionInputExtended input){
    size_t row_count = 0;
    size_t column_count = 0;
    vector<num_type> matrix;
    vector<num_type> T_matrix;

    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
       row_count = generate_rand_length(input.min_length, input.max_length);
       column_count = generate_rand_length(input.min_length, input.max_length);
    }

    if(input.algebra_object_version == AlgebraObjectVersion::kIncorrect){
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Unsupported AlgebraObjectVersion index: ", static_cast<int>(input.algebra_object_version)));
    }
    resize_and_generate_matrix(matrix, row_count, column_count, input.algebra_object_version, input.min_value, input.max_value);
    T_matrix = transpose_matrix(matrix, row_count, column_count);
    bool result = true;
    for(size_t i=0;i<column_count;++i){
        for(size_t j=0;j<row_count;++j){
            result = result && (T_matrix[i*row_count+j] == matrix[j*column_count+i]);
            if(! result){
                return expect::expect_eq(i, j);
            }
        }
    }
    return expect::expect_true(result);
}

ExpectationResult test_qr_decomposition(TestFunctionInputExtended input){
    vector<num_type> matrix;
    vector<num_type> Q_matrix;
    vector<num_type> R_matrix;

    // generation
    size_t row_count = 0;
    size_t column_count = 0;
    if(input.algebra_object_version != AlgebraObjectVersion::kEmpty && static_cast<int>(input.algebra_object_version) < static_cast<int>(FunctionOptimizationType::kBlock)){
        column_count = kUnrollCoefficient * generate_rand_length(input.min_length / kUnrollCoefficient, input.max_length / kUnrollCoefficient);
        row_count = kUnrollCoefficient * generate_rand_length(column_count / kUnrollCoefficient, input.max_length / kUnrollCoefficient);
    }else{
        if(input.algebra_object_version != AlgebraObjectVersion::kEmpty){
            // column_count = kBlockSize * generate_rand_length(1, 2);
            // row_count = kBlockSize * generate_rand_length(1, 2);

            column_count = kBlockSize * 2;
            row_count = kBlockSize * 2;
        }
    }
    column_count = row_count;
    //----------------------------------------
    // column_count = row_count;
    //----------------------------------------
    resize_and_generate_matrix(matrix, row_count, column_count, input.algebra_object_version, input.min_value, input.max_value);
    resize_and_generate_matrix(Q_matrix, row_count, column_count);
    resize_and_generate_matrix(R_matrix, column_count, column_count);

    std::function<void (const vector<num_type>&, vector<num_type>&, vector<num_type>&, size_t, size_t)> foo;
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        foo = QR_decomposition_base_simple;
        break;
    case FunctionOptimizationType::kRow:
        foo = QR_decomposition_row_product_simple;
        break;
    case FunctionOptimizationType::kRowRow:
        foo = QR_decomposition_row_product_matrix_process_simple;
        break;
    case FunctionOptimizationType::kSimd:
        foo = QR_decomposition_simd;
        break;
    case FunctionOptimizationType::kUnrolling:
        foo = QR_decomposition_unrolling;
        break;
    case FunctionOptimizationType::kDoubleUnrolling:
        foo = QR_decomposition_double_unrolling;
        break;
    case FunctionOptimizationType::kBlock:
        foo = QR_decomposition_block;
        break;
    case FunctionOptimizationType::kBlockScalar:
        foo = QR_decomposition_block_scalar;
        break;
    case FunctionOptimizationType::kInline:
        foo = QR_decomposition_block_scalar_inline;
        break;
    case FunctionOptimizationType::kMatrix:
        foo = QR_decomposition_full_matrix;
        break;
    case FunctionOptimizationType::kHouseholder:
        foo = QR_decomposition_base_householder;
        break;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Wrong FunctionOptimizationType index: ", static_cast<int>(input.function_type)));
    }
    if(input.algebra_object_version == AlgebraObjectVersion::kIncorrect){
        resize_and_generate_matrix(Q_matrix, row_count, column_count, input.algebra_object_version);
        resize_and_generate_matrix(R_matrix, column_count, column_count, input.algebra_object_version);
        vector_num normal_matrix, normal_Q, normal_R;
        resize_and_generate_matrix(normal_matrix, row_count, column_count);
        resize_and_generate_matrix(normal_Q, row_count, column_count);
        resize_and_generate_matrix(normal_R, column_count, column_count);
        Exception required_exception = Exception(ErrorType::kUnequalLengthError, "");
        bool wrong_matrix, wrong_Q, wrong_R;
        wrong_matrix = expect::expect_throw(foo, required_exception, matrix, normal_Q, normal_R, row_count, column_count);
        wrong_Q = expect::expect_throw(foo, required_exception, normal_matrix, Q_matrix, normal_R, row_count, column_count);
        wrong_R = expect::expect_throw(foo, required_exception, normal_matrix, normal_Q, R_matrix, row_count, column_count);
        return expect::expect_true(wrong_matrix && wrong_Q && wrong_R);
    }
    foo(matrix, Q_matrix, R_matrix, row_count, column_count);
    vector<num_type> test_matrix(row_count*column_count);
    matrix_product_base_simple(Q_matrix, R_matrix, test_matrix, row_count, column_count, column_count);
    return expect::expect_indexable_containers_near(matrix, test_matrix, kRelativeEps, row_count*column_count, true);
}

void resize_and_generate_matrix(vector_num& matrix, size_t row_count, size_t column_count, AlgebraObjectVersion matrix_version, num_type min_value, num_type max_value){
    if (matrix_version != AlgebraObjectVersion::kIncorrect){
        matrix.resize(row_count * column_count);
    }
    switch (matrix_version)
    {
    case AlgebraObjectVersion::kEmpty:
        return;
    case AlgebraObjectVersion::kZero:
        // to prevent memset of nullptr (even for null length)
        if(row_count == 0 || column_count == 0){
            return;
        }
        generate_zero_array(matrix.data(), row_count*column_count);
        return;
    case AlgebraObjectVersion::kIdentity:
        generate_identity_matrix(matrix.data(), row_count, column_count);
        return;
    case AlgebraObjectVersion::kGeneral:
        generate_rand_array(matrix.data(), row_count*column_count, min_value, max_value);
        return;
    case AlgebraObjectVersion::kIncorrect:
        if(generate_rand_length(0, 1)){
            matrix.resize(row_count * column_count + 1);
        }
        else{
            matrix.resize(row_count * column_count - 1);
        }
        return;
    default:
        throw Exception(ErrorType::kUnexpectedCase, generate_string("Unsupported AlgebraObjectVersion index for matrix generation: ", static_cast<int>(matrix_version)));
        break;
    }
}

