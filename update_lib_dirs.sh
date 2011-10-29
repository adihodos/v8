#/bin/bash

OUTPUT_DIR="/cygdrive/c/opt/libs/gfx_lib"
OUTPUT_SUBDIRS=("include/gfx" "lib/x64" "lib/x86")
INCLUDE_DIR="/include/gfx"
LIB_DIRX64="/x64"
LIB_DIRX86="/x86"
SOURCE_FILES_DIR="/cygdrive/e/source_code/gfx_lib/gfx_lib"
LIBFILES_DIRS=("/cygdrive/e/source_code/gfx_lib/build_output/Win32/Debug" \
               "/cygdrive/e/source_code/gfx_lib/build_output/Win32/Release" \
               "/cygdrive/e/source_code/gfx_lib/build_output/x64/Debug" \
               "/cygdrive/e/source_code/gfx_lib/build_output/x64/Release" \
               )
LIBFILES_DST_DIRS=("/x86" "/x86" "/x64" "/x64")

clean_directories() {
  if [[ ! -d "$OUTPUT_DIR" ]]
  then
    return
  fi

  $(rm -rf "$OUTPUT_DIR")
}

copy_headers() {
  for file_name in $(ls "$SOURCE_FILES_DIR" | egrep -i '*\.h$')
  do
    src_path="${SOURCE_FILES_DIR}/${file_name}"
    dst_path="${OUTPUT_DIR}/${INCLUDE_DIR}/${file_name}"
    #printf "\nWill copy %s to %s" "$src_path" "$dst_path"
    $(cp "$src_path" "$dst_path")
  done
}

copy_libraries() {
  index=0
  while [[ "$index" < "${#LIBFILES_DIRS[@]}" ]]
  do
    printf "\nProcessing library directory %s" "${LIBFILES_DIRS[$index]}"
    for libfile in $(ls -R "${LIBFILES_DIRS[$index]}" | egrep -i '*\.lib$')
    do
      src_file="${LIBFILES_DIRS[$index]}/lib/${libfile}"
      dst_file="${OUTPUT_DIR}/lib${LIBFILES_DST_DIRS[$index]}/${libfile}"
      #printf "\nWill copy %s to %s" "$src_file" "$dst_file"
      $(cp "$src_file" "$dst_file")
    done
    ((++index))
  done
}

create_directories() {
  for curr_dir in "${OUTPUT_SUBDIRS[@]}"
  do
    dir_full_path="${OUTPUT_DIR}/${curr_dir}"
    #printf "\nWill create directory %s" "$dir_full_path"
    $(mkdir -p "$dir_full_path")
  done

  copy_headers
  copy_libraries
}

clean_directories

create_directories
