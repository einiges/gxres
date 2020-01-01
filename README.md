# gxres - get X resources


## Description

Get the value of X resources, and optionally create a sourceable shell script.

For more information  
`man gxres`


## Example
```
$ xrdb -query
name.Class.resource: value
*color: #ffffff

$ gxres name.Class.resource
value

$ gxres -b 'bash_var_name:name.Class.resource' 'white:something.color'
export bash_var_name='value'
export white='#ffffff'
```


## Dependencies

Required libraries
* libx11


## Installation

Edit config.mk to to match your local setup. The default installation directory is '/usr/local'.
The Makefile provides support for DESTDIR and PREFIX variables.

Afterwards enter following command to build and install.
`make clean install`

