{
  description = "DendyTanks - a retro-style tank game";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = {
          default = pkgs.stdenv.mkDerivation {
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
          };
        };
        
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # Build tools
            cmake
            pkg-config
            gnumake
            ninja
            
            # Development tools
            gdb
            valgrind
            clang-tools # For clang-format, clang-tidy
            
            # Libraries
            SDL2
            SDL2_image
            libuuid
            gtest
          ];
          
          shellHook = ''
            echo "DendyTanks development environment"
            echo "Run 'cmake -B build && cmake --build build' to build the project"
          '';
        };
      }
    );
}