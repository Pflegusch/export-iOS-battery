# Description
A program written in C that extracts current battery information from a jailbroken iOS device that has ``bstat`` installed. The tool works by using ssh to get into the device and running the above program, the return value of that gets written into a local file that one can further use to regex the desired values out of. 
## Example usage
``docker-compose up -d && ./ssh_battery 192.168.178.29 id_rsa.pub``. This command additionally starts up an nginx webserver at the local port 80 which hosts the file we write our ssh output into. In the next step one can use Rainmeter to extract the desired values via regex and show them in a Rainmeter skin. 

## Rainmeter example
The following example will use regex to extract the current battery value as well as the current (dis-)charge value. One can then create a MeterBar or similar to create a skin as below.
````
[MeasureBattery]
Measure=WebParser
URL=http://localhost:80
RegExp=((?<=Current Battery: ).*)%
UpdateRate=5
MinValue=1
MaxValue=100

[MeasureDischarge]
Measure=WebParser
URL=http://localhost:80
RegExp=((?<=Battery Discharge: ).*)mA
UpdateRate=5
````
![alt text](https://github.com/pflegusch/export-iOS-battery/blob/main/rainmeter_ssh.PNG?raw=true)