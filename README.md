# Z boson mass reconstruction

This is a College project (third year Bsc in physics).

This code simulates a particle detector made of a calorimeter which return the particles energy and a tracker which reconstruct the trajectories and return the momentum of the particles. The goal is to calculate the mass of the Z boson using the muons generation produce from the Z desintegration.

This algorithm is based on Monte-Carlo and rejection sampling techniques.

### Initial conditions
The ICs are located in the data.txt file. The available parameters are the following:
* number of desintegrations
* detector acceptance
* bias on the measure of the θ angle (muons)
* resolution related to momentum
* resolution related to angles

### Build and run
create a directory
```
$ mkdir z_boson
$ cd z_boson
```

clone the code
```
$ git clone https://github.com/mainicolas/Z_boson_mass_reconstruction.git
```

run the code
```
$ ./main.exe
```


