generate_build_variant() {
    local value=`cat build_target.json`

    for i in `seq 1 2 ${#@}`
    do
      let n=i+1
      value=$(sed "s/${!i}/${!n}/g" <<< $value)
    done

    echo "${value}"
}

debug_variant=$( generate_build_variant '$1' Debug '$2' ${VERSION} '$3' *.exe,*.pdb )
release_variant=$( generate_build_variant '$1' Release '$2' ${VERSION} '$3' *.exe )

MATRIX_JSON="{\"build_target\": [\"Debug\", \"Release\"], \"include\": [${debug_variant}, ${release_variant}]}"

echo "::set-output name=matrix::${MATRIX_JSON}"