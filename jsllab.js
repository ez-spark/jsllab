function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

class Model{
    constructor(filename){
        
        this.filename = filename;
        this._filename = _malloc(4);
        
        _wasm_string_allocation(this._filename, this.filename.length+1);
        for(var i = 0; i < this.filename.length; i++){
            _wasm_insert_character_in_str(this._filename, this.filename.length, i, new TextEncoder().encode(this.filename)[i]);
        }
        _wasm_string_insert_terminator(this._filename, this.filename.length+1, i);
        
        this._model = _malloc(4);
        
        try{
            _wasm_load_model(this._model, this._filename);
        }
        catch{
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
        this.number_of_models = 0;
        this._models = null;
        
    }
    
    free(){
        try{
            _wasm_free_model(this._model);
            if(number_of_models > 0){
                _wasm_free_multiple_model_without_learning_parameters(this._models,this.number_of_models);
                _free(this._models);
            }
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
        catch{
            if(this.number_of_models > 0){
                _free(this._models);
            }
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
    }
    
    make_multi_thread(number_of_threads){
        if(number_of_threads < 1 || this.number_of_models > 0)
            return;
        this._models = _malloc(4);
        _wasm_copy_multiple_model_without_learning_parameters(this._model, this._models, number_of_threads);
    }
    
    reset(){
        _wasm_reset_model(this._model);
        _wasm_reset_multiple_model_without_learning_parameters(this._models, this,number_of_threads);
    }
    
    single_feed_forward(inputs, tensor_depth, tensor_i, tensor_j){
        var c_inputs = _malloc(4);
        var length = tensor_depth*tensor_i*tensor_j;
        _wasm_init_float_array(c_inputs, length);
        for(var i = 0; i < length; i++){
            _wasm_fill_float_array(c_inputs, i, inputs[i]);
        }
        
        _wasm_feed_forward(this._model, tensor_depth, tensor_i, tensor_j, c_inputs);
        
        var output_dim =  _wasm_get_output_dimension_from_model(this._model);
        var output = [];
        for(var i = 0; i < output_dim; i++){
            output.push(_wasm_get_float_value_from_model_output(this._model, i));
        }
        
        _wasm_free_array(c_inputs);
        _free(c_inputs);
        return output;
    }
    
    multiple_feed_forward(inputs, tensor_depth, tensor_i, tensor_j){
        if (this.number_of_threads < 1){
            return null;
        }
        var c_inputs = _malloc(4);
        var length = tensor_depth*tensor_i*tensor_j;
        _wasm_init_float_matrix(c_inputs, this.number_of_threads, this._models, tensor_depth, tensor_i, tensor_j, c_inputs);
        
        for(var i = 0; i < this.number_of_threads; i++){
            for(var j = 0; j < length; j++){
                _wasm_fill_float_matrix(c_inputs, i, j, inputs[i][j]);
            }
        }
        
        _wasm_feed_forward_opt_multi_thread(this._model , this._models, this.number_of_threads, tensor_depth,tensor_i,tensor_j,c_inputs);
        
        var output_dim = _wasm_get_output_dimension_from_model(this._model);
        var output = [];
        for(var i = 0; i < this.number_of_threads; i++){
            var list = [];
            for(var j = 0; j < output_dimension; j++){
                list.push(_wasm_get_float_value_from_models_output(this._models, i, j));
            }
            output.push(list);
        }
        _wasm_free_matrix(c_inputs, this.number_of_threads);
        _free(c_inputs);
        return output;
        
    }
    
}



class DQN{
    constructor(filename){
        
        this.filename = filename;
        this._filename = _malloc(4);
        
        _wasm_string_allocation(this._filename, this.filename.length+1);
        for(var i = 0; i < this.filename.length; i++){
            _wasm_insert_character_in_str(this._filename, this.filename.length, i, new TextEncoder().encode(this.filename)[i]);
        }
        _wasm_string_insert_terminator(this._filename, this.filename.length+1, i);
        
        this._model = _malloc(4);
        
        try{
            _wasm_load_dueling_categorical_dqn(this._model, this._filename);
        }
        catch{
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
    }
    
    free(){
        try{
            _wasm_free_dueling_categorical_dqn(this._model);
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
        catch{
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
    }

    reset(){
        _wasm_reset_dueling_categorical_dqn(this._model);
    }
    
    q_function(inputs, input_size, output_size){
 
        var c_inputs = _malloc(4);
        var length = input_size;
        _wasm_init_float_array(c_inputs, length);
        for(var i = 0; i < length; i++){
            _wasm_fill_float_array(c_inputs, i, inputs[i]);
        }
        
        var c_outputs = _malloc(4);
        _wasm_init_float_array(c_outputs,output_size);
        
        _wasm_compute_q_function(this._model, c_inputs, c_outputs, input_size,output_size);
        
        var output_dim =  output_size;
        var output = [];
        for(var i = 0; i < output_dim; i++){
            output.push(_wasm_get_float_value_from_array(this.c_outputs, i));
        }
        
        _wasm_free_array(c_inputs);
        _wasm_free_array(c_outputs);
        _free(c_inputs);
        _free(c_outputs);
        return output;
    }
    
    get_best_action(q_function){
        var index = 0;
        var max = q_function[0];
        for(var i = 1; i < q_function.length; i++){
            if(q_function[i] > max){
                max = q_function[i];
                index = i;
            }
        }
        return index;
    }
}

class Genome{
    constructor(filename, input_size, output_size){
        
        this.filename = filename;
        this.input_size = input_size;
        this.output_size = output_size;
        this._filename = _malloc(4);
        
        _wasm_string_allocation(this._filename, this.filename.length+1);
        for(var i = 0; i < this.filename.length; i++){
            _wasm_insert_character_in_str(this._filename, this.filename.length, i, new TextEncoder().encode(this.filename)[i]);
        }
        _wasm_string_insert_terminator(this._filename, this.filename.length+1, i);
        
        this._model = _malloc(4);
        
        try{
            _wasm_load_genome(this._model, this._filename, input_size, output_size);
        }
        catch{
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
    }
    
    free(){
        try{
            _wasm_free_genome(this._model);
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
        catch{
            _wasm_free_array(this._filename);
            _free(this._filename);
            _free(this._model);
        }
    }

    ff(inputs){
        
        var c_inputs = _malloc(4);
        var c_outputs = _malloc(4);
        var length = this.input_size;
        _wasm_init_float_array(c_inputs, length);
        for(var i = 0; i < length; i++){
            _wasm_fill_float_array(c_inputs, i, inputs[i]);
        }
        _wasm_ff_genome(this._model, c_inputs,c_outputs);
        
        var output_dim =  this.output_size;
        var output = [];
        for(var i = 0; i < output_dim; i++){
            output.push(_wasm_get_float_value_from_array(c_outputs, i));
        }
        
        _wasm_free_array(c_inputs);
        _wasm_free_array(c_outputs);
        _free(c_inputs);
        _free(c_outputs);
        return output;
        
    }
}

