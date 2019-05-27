Weighted Suffix Array based on ST-PSA algorithm
===

<b>Basic Instructions:</b>

The shell command 'make' should compile this program.

After compilation the binary 'index' will be created in the working directory, e.g. you can call it in this directory via

```
  ./index -t ../exampledata/text.fa -z 8 -o output.txt
```
<b>Usage:</b>
```
Usage: index <options>
Standard (Mandatory):
	-t,	--text		<str>	Filename for the Weighted String.
	-o,	--output	<str>	Filename for Output.
	-z,	--threshold	<dbl>	cumulative weighted threshold.
```
