


### pip
install pip:
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py

python get-pip.py


### add pip path to mac os
sudo nano /etc/paths
add path
Hit control-x to quit.
Enter “Y” to save the modified buffer.
restart terminal
echo $Path




### protobuf
No module named google.protobuf

pip uninstall protobuf
pip uninstall google
pip install google
pip install protobuf

or try:
pip install google-cloud



### git clone submodule
git clone --recursive


git clone --recurse-submodules -j8 git://github.com/foo/bar.git


git submodule update --init --recursive






