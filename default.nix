{ stdenv, cmake, glm, libpng, SDL2 }:

stdenv.mkDerivation rec {
  name = "raytrace-${version}";
  version = "0.1";

  buildInputs = [ cmake glm libpng SDL2 ];
}
