import fnmatch
import os
import stat
import shutil
import re
import sys

class tree_node :
    def __init__(self, name, parent = None) :
        self.parent = parent
        self.name = name
        self.children = []
        self.visited = False

    def add_child(self, child) :
        self.children.append(child)
        return child


class directory_tree :
    def __init__(self, root = None) :
        self.root = root

    def mk_hierarchy(self, node, name) :
        parent_path = os.path.join(name, node.name)
        if node.visited is False :
            print '{:s}'.format(parent_path)
            #os.mkdir(parent_path)

        for child in node.children :
            self.mk_hierarchy(child, parent_path) 

    def make_hierarchy(self) :
        if root is None :
            return
        self.mk_hierarchy(root, root.name)

    def set_root_node(self, newRoot) :
        self.root = newRoot

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

    subdir_list = [ (2, [ 'include', 'v8', 'base', 'math']), 
                    (1, ['lib', 'X86', 'X64']) ]
    try :
        for sub_and_children in subdir_list :
            parent_root = dest_root
            k = 0
            for i in range(0, sub_and_children[0]) :
                parent_root = os.path.join(parent_root, sub_and_children[1][i])
                os.mkdir(parent_root)
                print '[created] -> {:s}'.format(parent_root)
                k += 1
            
            for i in range(k, len(sub_and_children[1])) :
                child_path = os.path.join(parent_root, sub_and_children[1][i])
                os.mkdir(child_path)
                print '[created] -> {:s}'.format(child_path)
    except IOError as err :
        print err
        return False

    return True

def copy_header_files(src_path, dst_path) :
    prog = re.compile(r"\.h$|\.inl$")
    header_dirs = ['base', 'math']
    dst_root_path = os.path.join(os.path.join(dst_path, 'include'), 'v8')

    for dir in header_dirs :
        src_root = os.path.join(os.path.join(src_path, 'v8'), dir)
        dst_root = os.path.join(dst_root_path, dir)
        fileList = os.listdir(src_root)
        for fe in fileList :
            if prog.search(fe) :
                fullPathSrc = os.path.join(src_root, fe)
                fullPathDst = os.path.join(dst_root, fe)
                try :
                    shutil.copyfile(fullPathSrc, fullPathDst)
                    print '[copied] -> {:s} -> {:s}'.format(fullPathSrc, fullPathDst)
                except IOError as copy_err :
                    print copy_err

def copy_library_files(src_path, dst_path) :
    dir_mappings = [('x86', 'X86'), 
                    ('amd64', 'X64')]

    for mapping in dir_mappings :
        src_dir_path = os.path.join(src_path, mapping[0])
        file_list = os.listdir(src_dir_path)
        for curr_file in file_list :
            file_src = os.path.join(src_dir_path, curr_file)
            file_dst = os.path.join(os.path.join(dst_path, mapping[1]), curr_file)
            try :
                shutil.copyfile(file_src, file_dst)
                print '[copied] -> {:s} -> {:s}'.format(file_src, file_dst)
            except :
                print 'Copy {:s} -> {:s} fail'.format(file_src, file_dst)

def show_usage() :
    print 'Usage is : copylib [source_dir] [dest_dir]'

if __name__ == "__main__" :
    if (len(sys.argv) != 3) :
        show_usage()
        sys.exit(-1)

    dst_path = os.path.join(sys.argv[2], 'v8')
    src_path = os.path.join(sys.argv[1], 'v8')
    
    removeDirs(dst_path)
    make_destination_directories(dst_path)
    copy_header_files(src_path, dst_path)
    copy_library_files(os.path.join(src_path, 'build_output'), 
                       os.path.join(dst_path, 'lib'))
