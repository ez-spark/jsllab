#include "./src/llab.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void EMSCRIPTEN_KEEPALIVE wasm_string_null(char** str){
    (*str) = NULL;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_string_allocation(char** str, int size){
    (*str) = (char*)malloc(sizeof(char)*size);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_insert_character_in_str(char** str, int size, int index, char c){
    if(index >= size)
        return;    
    (*str)[index] = c;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_string_insert_terminator(char** str, int size, int index){
    if(index >= size)
        return;
    (*str)[index] = '\0';
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_internal_free(void** s){
    free(*s);
    return;
}

char EMSCRIPTEN_KEEPALIVE wasm_get_char_from_str(char** str, int index){
    return (*str)[index];
}

void EMSCRIPTEN_KEEPALIVE wasm_init_float_array(float** a, int size){
    (*a) = (float*)malloc(sizeof(float)*size);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_init_int_array(int** a, int size){
    (*a) = (int*)malloc(sizeof(int)*size);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_fill_float_array(float** a, int index, float v){
    (*a)[index] = v;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_fill_int_array(int** a, int index, int v){
    (*a)[index] = v;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_init_char_matrix(char*** c, int rows, int cols){
    (*c) = (char**)malloc(sizeof(char*)*rows);
    int i;
    for(i = 0; i < rows; i++){
        (*c)[i] = (char*)malloc(sizeof(char)*cols);
    }
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_fill_char_matrix(char*** c, int row, int col, char ch){
    (*c)[row][col] = ch;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_init_float_matrix(float*** c, int rows, int cols){
    (*c) = (float**)malloc(sizeof(float*)*rows);
    int i;
    for(i = 0; i < rows; i++){
        (*c)[i] = (float*)malloc(sizeof(float)*cols);
    }
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_fill_float_matrix(float*** c, int row, int col, float v){
    (*c)[row][col] = v;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_init_int_matrix(int*** c, int rows, int cols){
    (*c) = (int**)malloc(sizeof(int*)*rows);
    int i;
    for(i = 0; i < rows; i++){
        (*c)[i] = (int*)malloc(sizeof(int)*cols);
    }
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_fill_int_matrix(int*** c, int row, int col, int v){
    (*c)[row][col] = v;
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_fill_terminator_matrix(char*** c, int row, int col){
    (*c)[row][col] = '\0';
    return;
}

int EMSCRIPTEN_KEEPALIVE wasm_get_int_value_from_array(int** array, int index){
    return (*array)[index];
}

float EMSCRIPTEN_KEEPALIVE wasm_get_float_value_from_array(float** array, int index){
    return (*array)[index];
}

void EMSCRIPTEN_KEEPALIVE wasm_free_matrix(void*** m, int rows){
    int i;
    for(i = 0; i < rows; i++){
        free((*m)[i]);
    }
    free(*m);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_free_array(void** m){
    free(*m);
    return;
}   

void EMSCRIPTEN_KEEPALIVE wasm_load_model(model** m, char** filename){
    (*m) = load_model(*filename);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_free_model(model** m){
    free_model(*m);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_copy_model_without_learning_parameters(model** m, model** c){
    (*c) = copy_model_without_learning_parameters(*m);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_copy_multiple_model_without_learning_parameters(model** m, model*** c, int size){
    int i;
    *c = (model***)malloc(sizeof(model**)*size);
    for(i = 0; i < size; i++){
        (*c)[i] = copy_model_without_learning_parameters(*m);
    }
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_free_multiple_model_without_learning_parameters(model*** c, int size){
    int i;
    for(i = 0; i < size; i++){
        free_model_without_learning_parameters((*c)[i]);
    }
    free(*c);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_reset_model(model** m){
    reset_model(*m);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_reset_model_without_learning_parameters(model** m){
    reset_model_without_learning_parameters(*m);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_feed_forward(model** m, int tensor_depth, int tensor_i, int tensor_j, float** inputs){
	model_tensor_input_ff((*m),tensor_depth,tensor_i,tensor_j,(*inputs));
}

void EMSCRIPTEN_KEEPALIVE wasm_feed_forward_witout_learning_parameters(model** m, model** m_wlp,  int tensor_depth, int tensor_i, int tensor_j, float** inputs){
    model_tensor_input_ff_without_learning_parameters((*m_wlp),(*m),tensor_depth,tensor_i,tensor_j,(*inputs));
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_reset_multiple_model_without_learning_parameters(model*** m, int size){
    int i;
    for(i = 0; i < size; i++){
        reset_model_without_learning_parameters((*m)[i]);
    }
    return;
}

int EMSCRIPTEN_KEEPALIVE wasm_get_output_dimension_from_model(model** m){
    return get_output_dimension_from_model(*m);
}

void EMSCRIPTEN_KEEPALIVE wasm_feed_forward_opt_multi_thread(model** m , model*** ms, int number_of_threads, int tensor_depth, int tensor_i, int tensor_j,float*** inputs){
    model_tensor_input_ff_multicore_opt(*ms, *m, tensor_depth, tensor_i, tensor_j, *inputs, number_of_threads, number_of_threads);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_load_dueling_categorical_dqn(dueling_categorical_dqn** dqn, char** filename){
    *dqn = load_dueling_categorical_dqn(*filename);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_free_dueling_categorical_dqn(dueling_categorical_dqn** dqn){
    free_dueling_categorical_dqn(*dqn);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_free_dueling_categorical_dqn_without_learning_parameters(dueling_categorical_dqn** dqn){
    free_dueling_categorical_dqn_without_learning_parameters(*dqn);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_free_multiple_dueling_categorical_dqn_without_learning_parameters(dueling_categorical_dqn*** dqn, int size){
    int i;
    for(i = 0; i < size; i++){
        free_dueling_categorical_dqn_without_learning_parameters((*dqn)[i]);
    }
    free(*dqn);
    return;
}

int EMSCRIPTEN_KEEPALIVE wasm_get_dqn_input_size(dueling_categorical_dqn** dqn){
    return get_input_layer_size_dueling_categorical_dqn(*dqn);
}

void EMSCRIPTEN_KEEPALIVE wasm_copy_dueling_categorical_dqn_without_learning_parameters(dueling_categorical_dqn** dqn, dueling_categorical_dqn** copy){
    *copy = copy_dueling_categorical_dqn_without_learning_parameters(*dqn);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_copy_multiple_dueling_categorical_dqn_without_learning_parameters(dueling_categorical_dqn** dqn, dueling_categorical_dqn*** copy, int size){
    int i;
    *copy = (dueling_categorical_dqn***)malloc(sizeof(dueling_categorical_dqn**)*size);
    for(i = 0; i < size; i++){
        (*copy)[i] = copy_dueling_categorical_dqn_without_learning_parameters(*dqn);
    }
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_reset_dueling_categorical_dqn(dueling_categorical_dqn** dqn){
    reset_dueling_categorical_dqn(*dqn);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_reset_dueling_categorical_dqn_without_learning_parameters(dueling_categorical_dqn** dqn){
    reset_dueling_categorical_dqn_without_learning_parameters(*dqn);
    return;
}

void EMSCRIPTEN_KEEPALIVE wasm_reset_multiple_dueling_categorical_dqn_without_learning_parameters(dueling_categorical_dqn*** dqn, int size){
    int i;
    for(i = 0; i < size; i++){
        reset_dueling_categorical_dqn_without_learning_parameters((*dqn)[i]);
    }
    return;
}

float EMSCRIPTEN_KEEPALIVE wasm_get_float_value_from_model_output(model** m, int index){
    return get_output_layer_from_model(*m)[index];
}

float EMSCRIPTEN_KEEPALIVE wasm_get_float_value_from_models_output(model*** m, int i, int index){
    return get_output_layer_from_model((*m)[i])[index];
}

void EMSCRIPTEN_KEEPALIVE wasm_compute_q_function(dueling_categorical_dqn** dqn, float** input, float** output, int input_size, int output_size){
    int i;
    compute_probability_distribution(*input, input_size, *dqn);
    float* q = compute_q_functions(*dqn);
    for(i = 0; i < output_size; i++){
        (*output)[i] = q[i];
    }
}

int EMSCRIPTEN_KEEPALIVE wasm_get_best_action_from_float(float** input, int input_size){
    int i, index = 0;
    float max = (*input)[0];
    for(i = 0; i < input_size; i++){
        if((*input)[i] > max){
            max = (*input)[i];
            index = i;
        }
    }
    return index;
}

int EMSCRIPTEN_KEEPALIVE wasm_get_best_action_from_dqn(dueling_categorical_dqn** dqn, float** input, float** output, int input_size, int output_size){
    int i;
    compute_probability_distribution(*input, input_size, *dqn);
    float* q = compute_q_functions(*dqn);
    int index = 0;
    float max = q[0];
    for(i = 0; i < output_size; i++){
        if(q[i] > max){
            max = q[i];
            index = i;
        }
    }
    return index;
}

void EMSCRIPTEN_KEEPALIVE wasm_load_genome(genome** g, char** filename, int input_size, int output_size){
    (*g) = load_genome_complete(*filename);
}

void EMSCRIPTEN_KEEPALIVE wasm_free_genome(genome** g){
    free_genome(*g, get_global_innovation_number_connections_from_genome(*g));
}

void EMSCRIPTEN_KEEPALIVE wasm_ff_genome(genome** g, float** inputs, float** output){
    *output = feed_forward(*g, *inputs,get_global_innovation_number_nodes_from_genome(*g), get_global_innovation_number_connections_from_genome(*g));
}
