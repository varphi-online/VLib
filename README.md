# VLib
Is a collection of [STB style](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt)
header libraries made with the intention of being spec-compliant and fast.

There is a top-level makefile that amalgamates all headers from each module into a build folder,
but most likely you only need one, which is why each module has its own Makefile with the default
`make` being the amalgamate.
