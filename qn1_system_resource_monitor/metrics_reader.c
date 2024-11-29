#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static PyObject *read_usage_file(PyObject *self, PyObject *args) {
    const char *filename;
    FILE *fp;
    char line[256];
    PyObject *result_list;

    // Parse the argument (filename)
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    fp = fopen(filename, "r");
    if (!fp) {
        PyErr_SetString(PyExc_FileNotFoundError, "Could not open file");
        return NULL;
    }

    // Create a Python list to store the results
    result_list = PyList_New(0);
    if (!result_list) {
        fclose(fp);
        return PyErr_NoMemory();
    }

    // Read each line from the file
    while (fgets(line, sizeof(line), fp)) {
        PyObject *record;
        char timestamp[10];
        double cpu, memory;
        unsigned long net_rx, net_tx;

        // Parse the line into variables
        if (sscanf(line, "%9s %lf %lf %lu %lu", timestamp, &cpu, &memory, &net_rx, &net_tx) != 5) {
            continue; // Skip malformed lines
        }

        // Create a Python tuple for the record
        record = Py_BuildValue("(s, f, f, k, k)", timestamp, cpu, memory, net_rx, net_tx);
        if (!record) {
            Py_DECREF(result_list);
            fclose(fp);
            return PyErr_NoMemory();
        }

        // Append the tuple to the list
        if (PyList_Append(result_list, record) < 0) {
            Py_DECREF(record);
            Py_DECREF(result_list);
            fclose(fp);
            return NULL;
        }

        Py_DECREF(record);
    }

    fclose(fp);
    return result_list;
}

// Define methods
static PyMethodDef MetricsMethods[] = {
    {"read_usage_file", read_usage_file, METH_VARARGS, "Read metrics from a file"},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Define the module
static struct PyModuleDef metricsmodule = {
    PyModuleDef_HEAD_INIT,
    "metrics", // Module name
    NULL,      // Module documentation
    -1,        // Size of per-interpreter state of the module
    MetricsMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_metrics(void) {
    return PyModule_Create(&metricsmodule);
}
