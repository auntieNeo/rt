{ stdenv, bison, cmake, flex, glm, libpng, SDL2 }:

stdenv.mkDerivation rec {
  name = "raytrace-${version}";
  version = "0.1";

  buildInputs = [ bison cmake flex glm libpng SDL2 ];
}
