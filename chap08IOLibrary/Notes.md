- [The IO Library](#the-io-library)
  - [The IO Classes](#the-io-classes)
    - [No copy or assign for IO objects](#no-copy-or-assign-for-io-objects)
    - [Condition states](#condition-states)
    - [Interogating the state of a stream](#interogating-the-state-of-a-stream)
    - [Managing the output buffer](#managing-the-output-buffer)
    - [Managing output buffer](#managing-output-buffer)
    - [Tying input and output streams](#tying-input-and-output-streams)
  - [File Input and Output](#file-input-and-output)
    - [Usage](#usage)
    - [```open``` and ```close```](#open-and-close)
    - [File modes](#file-modes)
  - [```string``` Streams](#string-streams)

# The IO Library
## The IO Classes
IO library types and headers
| Header | Type |
| --- | :--- |
| ```iostream``` | ```istream```, ```wistream``` reads from a stream                 |
|                | ```ostream```, ```wostream``` writes to a stream                  |
|                | ```iostream```, ```wiostream``` reads and writes a stream         |
| ```fstream```  | ```ifstream```, ```wifstream``` reads from a file stream          |
|                | ```ofstream```, ```wofstream``` writes to a file stream           |
|                | ```fstream```, ```wfstream``` reads and writes a file stream      |
| ```sstream```  | ```istringstream```, ```wistringstream``` reads from a string     |
|                | ```ostringstream```, ```wostringstream``` writes to a string      |
|                | ```stringstream```, ```wstringstream``` reads and writes a string |

The ```w-```streams are intended for reading wide char (wchar_t).

### No copy or assign for IO objects
IO objects can not be copied or assigned to another.
```c++
ostream out1, out2;
out1 = out2;           // error, no assign for IO object
ostream out3(out1);    // error, no copy for IO object
```

### Condition states
IO classes define functions and flags that let us access and manipulate the condiction state of a stream. The eaist way to determine the state of a stream is to use the object as a condition.
```c++
// while cin in good state, the body will run
while ( cin >> word )
    // do ...
```

### Interogating the state of a stream
The IO library defines a type named ```iostate``` that it uses to convey information about the state of a stream.
| iostate | description |
| --- | :--- |
| ```badbit``` | system-level failure, e.g. unrecoverable read or write error |
| ```failbit``` | recoverable error, e.g. reading a character when numeric data was expected |
| ```eofbit``` | end-of-file |
| ```goodbit``` | no failures on the stream |
The library also defines a set of functions to interrogate the state of these flags.
| function | description |
| --- | :--- |
| s.eof()  | ```true``` if ```eofbit``` in the stream s is set                  |
| s.fail() | ```true``` if ```failbit``` or ```badbit``` in the stream s is set |
| s.bad()  | ```true``` if ```badbit``` in the stream s is set                  |
| s.good() | ```true``` if the stream s is in a valid state                     |
| s.clear() | Reset all condiction values in the stream s to valid state. |
| s.clear(flags) | Reset the condition of s to flags. Type of flags is one of ```iostate``` |
| s.setstate(flags) | Adds specified condiction to s |
| s.rdstate() | Returns current condition of s as a ```iostate``` value |

### Managing the output buffer
Here is an example to use the functions above
```c++
auto old_state = cin.rdstate(); // remember the current state of cin
cin.clear();                    // make cin valid
process_input(cin);             // use cin
cin.setstate(old_state);        // now reset cin to its old state
```

### Managing output buffer
Each output stream manages a buffer, which holds output data temperarily. Buffers will be flushed and write data to the output target when needed or requested.
* Flushing the output buffer:
    ```c++
    cout << " " << endl;  // writes string and a newline, then flushes the buffer
    cout << " " << flush; // writes string, then flushes the buffer; adds no data
    cout << " " << ends;  // writes string and a null, then flushes the buffer
    ```
* The ```unitbuf``` manipulator: this manipulator tells the stream to do a flush after every subsequent write.
    ```c++
    cout << unitbuf; // all writes will be flushed immediately
    cout << nounibuf; // returns to normal buffering
    ```

### Tying input and output streams
Sometimes, when we use one stream, we want to flush anther buffer beforehand. This pretends streams to interrupt each other. For example, ```cout``` is tied to ```cin```, so when ```cin``` is used, ```cout``` is flushed beforehand. The tie relation can also be changed.
```c++
cin.tie(&cout); // illustration only, cin is already tied to cout
ostream *old_tie = cin.tie(nullptr);
cin.tie(&cerr); // tie cin to cerr
cin.tie(old_tie); // reestablish normal tie between cin and cout
```
Each stream can be tied to at most one stream at a time, but multiple streams can tie themselves to the same ostream.

## File Input and Output
The ```fstream``` header defines ```ifstream```, ```ofstream``` and ```fstream```, all of them can be used similar to standard IO streams.
### Usage
```c++
ifstream in(file); // construct an input file stream, file can be a c++ string or a c-like char*
```
### ```open``` and ```close```
We can construct a file stream object that is not associated to any file, and open a file later. The object can be reused.
```c++
ifstream inobj;       // a file stream object not associated to any file
inobj.open(filename); // open file
inobj.close();        // close
inobj.open(filename2); // open another file
```

### File modes
When a file stream opens a file, we have several file modes. Some of them can be used together.
| File Modes | Description |
| --- | :--- |
| ```in```   | Open for input |
| ```out```  | Open for output, implicitly ```trunc``` |
| ```app```  | Write to the end of file |
| ```ate```  | Seek to the end immediately after the open |
| ```trunc``` | Truncate the file |
| ```binary``` | DO IO operations in binary mode |

## ```string``` Streams
The header file ```sstream``` defines ```istringstream```, ```ostringstream``` and ```stringstream```, that can be used similar to standard IO streams.

