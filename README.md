# Energenie controller APP

![app.png](app.png)

Toggle Energenie smart sockets from your wrist.

    Pebble (C) --kitjs--> Pebble(JS) --request--> RaspberryPI --radio--> Energenie

Requires external hardware/software to convert API requests into radio signals compatible with Energenie (aka Raspberry PI):
[Link to webapp for Energenie](https://github.com/rephus/energenie-manager)

Based on [Pebble example menu layer](`https://github.com/pebble-examples/feature-menu-layer`)

# Enable pebble developer connection on the phone

* Open the official pebble app on your phone
* Menu > Developer Connection
* Enable the connection
* REcord the IP address (you'll need it later to upload your app )

# Pebble installation

More info: https://developer.pebble.com/sdk/install/linux/

Setup pebble env
```
sudo apt-get install libsdl1.2debian libfdt1 libpixman-1-0 #emulator dependencies

virtualenv --no-site-packages .env
source .env/bin/activate
pip install -r requirements.txt
```

Setup aliases
```
function pebble-env {
        cd '/home/javier/apps/pebble-sdk-4.5-linux64'
        source .env/bin/activate
}
alias pebble='/home/javier/apps/pebble-sdk-4.5-linux64/bin/pebble'
alias pebble-install='pebble install --logs --emulator basalt'
```

Enable dev mode on pebble app (settings) to do
```
alias pebble-install='pebble install --phone <ip> --logs'
```

Build and launch
```
pebble build && pebble-install
```
