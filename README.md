Add custom Jupyter-Notebook kernel: 

```
$ mkdir ~/.local/share/jupyter/kernels/visionary-dls
```

Create a file `kernel.json` with the following contents:

```json
{
 "display_name": "Python 2 visionary-dls", 
 "language": "python", 
 "argv": [
  "/path/to/code/kernel_wrapper.sh",
  "-m", 
  "ipykernel_launcher", 
  "-f", 
  "{connection_file}"
 ]
}
```
