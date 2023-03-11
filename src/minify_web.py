#!/usr/bin/python
import requests
import base64
import os

input_dir = "html"
# f_output = open("include/esp32_socket_webpage.h", "w")
f_output = open("src/esp32_socket_webpage.h", "w")
f_output.write("#ifndef ESP32_SOCKET_WEBPAGE_H\n#define ESP32_SOCKET_WEBPAGE_H\n\n")
f_output.write("#ifndef Arduino_h\n  #include <Arduino.h>\n#endif\n\n")

URL_minify_js   = 'https://javascript-minifier.com/raw' # Website to minify javascript
URL_minify_html = 'https://www.toptal.com/developers/html-minifier/raw'        # Website to minify html
URL_minify_css  = 'https://cssminifier.com/raw'         # Website to minify css

def write_to_file(file, data, dir=""):
    filename, file_extension = os.path.splitext(file)       # Split filename and file extension
    file_extension = file_extension.replace(".","")         # Remove puncuation in file extension

    dir = dir.replace(input_dir,"")                         # Remove the first directory(input_dir)
    dir = dir.replace("\\","/")                             # Chang to /
    f_output.write("// " + dir + "\n")                      # Print comment
    f_output.write("const char* data_" + filename + "_" + file_extension + "_path PROGMEM = \""+str(dir)+"\";\n")    # print path
    f_output.write("const char data_"+filename+"_"+file_extension+"[] PROGMEM = {"+data.upper()+"};\n\n")            # print binary data

    # f_output.write("#define data_" + filename + "_len " + str(data.count('0x')) +"\n")

def aschii2Hex(text):
    output_str = ""
    x = 1
    strLen = len(text)
    for character in text:
        output_str += hex(ord(character))

        if (x != strLen):
            output_str += ","
        x += 1
    output_str = output_str[:-5] # fix for erroneous last group 0x3E
    return output_str

def minify_js(input_file):
    url = URL_minify_js
    data = {'input': open(input_file, 'rb').read()}
    return requests.post(url, data=data).text

def minify_html(input_file):
    url = URL_minify_html
    data = {'input': open(input_file, 'rb').read()}
    return requests.post(url, data=data).text

def minify_css(input_file):
    url = URL_minify_css
    data = {'input': open(input_file, 'rb').read()}
    return requests.post(url, data=data).text


for root, dirs, files in os.walk(input_dir, topdown=False):
    for name in files:   # for files
        path = os.path.join(root, name)
        if name.endswith(".js"):
            print(os.path.join(root, name))
            minified_js = minify_js(os.path.join(root, name))          # minify javascript
            hexified_js = aschii2Hex(minified_js)                      # convert to hex
            write_to_file(name, hexified_js, os.path.join(root, name)) # write to file
            continue

        elif name.endswith(".html"):
            print(os.path.join(root, name))
            minified_html = minify_html(os.path.join(root, name))        # minify html
            print(minified_html)
            hexified_html = aschii2Hex(minified_html)                 # convert to hex
            write_to_file(name, hexified_html, os.path.join(root, name)) # write to file
            continue

        elif name.endswith(".css"):
            print(os.path.join(root, name))
            minified_css = minify_css(os.path.join(root, name))         # minify css
            hexified_css = aschii2Hex(minified_css)                     # convet to hex
            write_to_file(name, hexified_css, os.path.join(root, name)) # write to file


f_output.write("#endif\n\n")
f_output.close()
