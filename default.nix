{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
    name = "rainbow-joe-git";

    src = pkgs.fetchFromGitHub {
        owner = "mupfelofen-de";
        repo = "rainbow-joe";
        rev = "68cd98ac0cdbeb8f78e650b13ae0efa5c726f56e";
        sha256 = "0w1jbkshzxvphmis1gsb91v5qk1ckjdhcg1ywfhgbqw73s87vn2g";
    };

    buildInputs = with pkgs; [
        libxml2
        SDL2
        SDL2_image
        SDL2_mixer
        zlib
    ];

    installPhase = ''
        install -Dsm 755 rainbow-joe $out/bin/rainbow-joe
    '';
}
