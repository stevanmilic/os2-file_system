myTestBuild: 
	cl /EHsc /I fileSystem\\h /I particija-VS2015 fileSystem\\src\\*.cpp /link particija-VS2015\\part.lib
clean:
	rm *.exe *.obj _CL_*
