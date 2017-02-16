 #!/bin/bash
echo "Remember to open your Pebble phone app > Menu > Developer Connection"
pebble build && pebble install --phone 192.168.2.20 --logs
