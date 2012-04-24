import fnmatch
import os
import stat
import shutil
import re
import sys

def removeDirs(dirPath) :
    try :
        os.chmod(dirPath, stat.S_IWRITE | stat.S_IREAD)
        shutil.rmtree(dirPath)
        print 'Destination directory {:s} successfully removed.'.format(dirPath)
    except :
        print 'Failed to remove directory {:}, you should manually delete it'.format(dirPath)

def make_destination_directories(dest_root) :
    try :
        os.mkdir(dest_root)
        print 'Directory {:s} created.'.format(dest_root)
    except :
        print 'Failed to create directory {:s}'.format(dest_root)
        return False

    subdir_list = [ ['include', 'gfx'], ['lib', 'X86', 'X64'] ]
    try :
        for sub_and_children in subdir_list :
            parent_root = os.path.join(dest_root, sub_and_children[0])
            os.mkdir(parent_root)
            for i in range(1, len(sub_and_children)) :
                full_path = os.path.join(parent_root, sub_and_children[i])
                os.mkdir(full_path)
                print 'Directory {:s} created.'.format(full_path)
    except :
        print 'Failed to create subdirectories'
        return False

    return True

def copy_header_files(src_path, dst_path) :
    prog = re.compile(r"\.h$|\.inl$")
    src_root = os.path.join(src_path, 'gfx_lib')
    dst_root = os.path.join(os.path.join(dst_path, 'include'), 'gfx')
    fileList = os.listdir(src_root)
    for fe in fileList :
        if prog.search(fe) :
            fullPathSrc = os.path.join(src_root, fe)
            fullPathDst = os.path.join(dst_root, fe)
            try :
                shutil.copyfile(fullPathSrc, fullPathDst)
                print '{:s} -> {:s}'.format(fullPathSrc, fullPathDst)
            except IOError as copy_err :
                print copy_err

def copy_library_files(src_path, dst_path) :
    dir_mappings = [('win32\\lib', 'X86'), 
                    ('x64\\lib', 'X64')]

    for mapping in dir_mappings :
        src_dir_path = os.path.join(src_path, mapping[0])
        file_list = os.listdir(src_dir_path)
        for curr_file in file_list :
            file_src = os.path.join(src_dir_path, curr_file)
            file_dst = os.path.join(os.path.join(dst_path, mapping[1]), curr_file)
            try :
                shutil.copyfile(file_src, file_dst)
                print '{:s} -> {:s}'.format(file_src, file_dst)
            except :
                print 'Copy {:s} -> {:s} fail'.format(file_src, file_dst)

def show_usage() :
    print 'Usage is : copylib [source_dir] [dest_dir]'

if __name__ == "__main__" :
    if (len(sys.argv) != 3) :
        show_usage()
        sys.exit(-1)

    dst_path = os.path.join(sys.argv[2], 'gfx_lib')
    src_path = os.path.join(sys.argv[1], 'gfx_lib')
    removeDirs(dst_path)
    make_destination_directories(dst_path)
    copy_header_files(src_path, dst_path)
    copy_library_files(os.path.join(src_path, 'build_output'), 
                       os.path.join(dst_path, 'lib'))
