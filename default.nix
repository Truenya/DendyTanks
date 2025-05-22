{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
  pname = "dendy-tanks";
  version = "0.1.0";
  
  src = ./.;
  
  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
  ];
  
  buildInputs = with pkgs; [
    SDL2
    SDL2_image
    libuuid
    gtest
  ];
  
  configurePhase = ''
    cmake -B build .
  '';
  
  buildPhase = ''
    cmake --build build
  '';
  
  installPhase = ''
    mkdir -p $out/bin
    cp build/ssdl $out/bin/dendy-tanks
    
    # Copy resources
    mkdir -p $out/share/dendy-tanks
    cp -r resources $out/share/dendy-tanks/
  '';
  
  meta = with pkgs.lib; {
    description = "Dendy Tanks game";
    homepage = "https://github.com/Truenya/DendyTanks";
    license = licenses.mit;
    platforms = platforms.all;
    maintainers = [];
  };
}