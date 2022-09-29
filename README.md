# jsllab

jsllab is the inference library of Pyllab in javascript.

- jsllab allows you to run: model, dueling_categorical_dqn and genome classes trained with Pyezspark or Pyllab or llab.

# To run your models you need to compile

Don't worry you are covered: use docker (i will you explain the steps).

Since jsllab read your models directly from webassembly you need
to embed the .bin files during the compilation. 

From the compilation will be generated a .data file where your models have been embedded

- Download the repo

- Put your binary files in the root directory (where is located this readme)

- Install docker from https://docs.docker.com/install/

- Open the terminal as root

- Download a pre-built docker image:

```
docker pull webassembly/toolchain
```
- Run the image in a container:

```
docker run -it --net=host --rm -v <ABSOLUTE PATH OF THIS README>:/project webassembly/toolchain:latest /bin/bash --login
```

- Update The version of emscripten (from here you are in the bash of the docker)
```
cd emsdk
git reset --hard
git pull
./emsdk install latest
./emsdk activate latest
source emsdk_env.sh
```

- Set python 3 as dafault python instead of python2.7 to run new emscripten code:

- First install vim
```
apt-get install vim
```
- Then
```
vim ~/.bashrc
```

- Add this line to the file you opened (~/.bashrc)
```
alias python=python3
```

- To save and exit from vim :wq + ENTER

- Run the file with source:
```
source ~/.bashrc
```

- Install all the needed dependencies (can take several minutes):

```
apt-get update
apt-get install python3 -y
apt install python3-pip
```

- Check the version of emscripten, should be > 2.0.0

```
emcc -v
```

- If you are in emsdk:
  
```
cd ..
```

- Now compile:

```
cd project
make
```

From the compilation will be generated:

- jsllab_handler.data
- jsllab_handler.js
- jsllab_handler.wasm
- jsllab_handler.worker.js

# Run your models

In order to run your models you must import in your html file:

- jsllab_handler.js
- jsllab.min.js

jsllab.handler.js will fetch and import all the other files you have created during the compilation.

*In order to let jsllab.handler.js to retrieve all the needed files you need to put all these files under the
same http path*

For example: if to retrieve jsllab_handler.js you use http://domain.com/src/jsllab_handler.js

then http://domain.com/src/jsllab_handler.data, http://domain.com/src/jsllab_handler.wasm, http://domain.com/src/jsllab_handler.worker.js 
are the paths for the other files.

# SharedArrayBuffer overview

Since we are using multithread wasm code we need the SharedArrayBuffer flag to be activated.

- If the url for your code is on localhost, you just need to have the server sending the headers:

  - Cross-Origin-Opener-Policy: same-origin
  - Cross-Origin-Embedder-Policy: require-corp

- If you are using another url you need the https and the previous headers too.

# How to run the models

- To run models


```
var model = new Model("filename.bin");
var l = [1,1,1];
// for forwarding
var output = model.single_feed_forward(l,1,1,3);
// for multi thread forwarding
var n_threads = 3;
model.make_multi_thread(n_threads);
var c = [[1,1,1],[1,1,1],[1,1,1]]
var outputs = m.multiple_feed_forward(c, 1, 1, 3);
// for resetting
m.reset();
// for freeing
m.free();
```

- To run DQNs

```
var dqn = new DQN("filename.bin");
l = [1,1,1];
// for q functions
var q = dqn.q_function(l, 3, 10);
// for getting best action index
var action = dqn.get_best_action(q);
// for resetting
dqn.reset();
// for freeing
dqn.free();
```

- To run Genome

```
var input_size = 3;
var output_size = 10;
var g = new Genome("filename.bin",input_size, output_size);
var l = [1,1,1]
// for forwarding
var output = g.ff(l);
// for freeing
g.free();
```







