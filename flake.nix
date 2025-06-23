{
  description = "LLVM dev shell";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.${system}.default = pkgs.mkShell {
        name = "llvm-dev-shell";

        packages = [
          pkgs.clang-tools_17
          pkgs.llvm_17
          pkgs.lld_17
          pkgs.clang_17
          pkgs.cmake
          pkgs.ninja
        ];

        shellHook = ''
          export CC=clang
          export CXX=clang++
          export LLVM_DIR=${pkgs.llvm_17}/lib/cmake/llvm
          export LLD_DIR=${pkgs.lld_17}/lib/cmake/lld
          echo "LLVM ${pkgs.llvm_17.version} dev shell ready!"
        '';
      };
    };
}