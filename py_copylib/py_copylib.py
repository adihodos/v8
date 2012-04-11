import fnmatch
import os
import stat
import shutil
import re

destinationDirectories = [ ('C:\\opt\\dev\\libs\\gfx_lib', 
                            ['include', 'gfx'], ['lib', 'X86', 'X64']) ]

sourceDirectory = "D:\\kits\\src_code\\gfx_lib\\gfx_lib"
sourceDirectoryLibs = 'D:\\kits\\src_code\\gfx_lib\\build_output'

def removeDirs(dirPath) :
    try :
        os.chmod(dirPath, stat.S_IWRITE | stat.S_IREAD)
        shutil.rmtree(dirPath)
        print 'Destination directory {:s} successfully removed.'.format(dirPath)
    except :
        print 'Failed to remove directory {:}, you should manually delete it'.format(dirPath)

def makeDirs(dirPathList) :
    try :
        for path in dirPathList :
            os.mkdir(path[0])
            print 'Directory {:s}'.format(path[0])
            
            for i in range(1, len(path)) :
                dirList = path[i]
                parentDir = os.path.join(path[0], dirList[0])
                os.mkdir(parentDir)
                print 'Directory {:s} created.'.format(parentDir)
                for j in range(1, len(dirList)) :
                    dirPath = os.path.join(parentDir, dirList[j])
                    os.mkdir(dirPath)
                    print 'Directory {:s} created'.format(dirPath)
            
        print 'Destination directories successfully created.'
    except :
        print 'Failed to create directory'

def copyHeaderFiles(srcPath, destPath) :
    print 'La la la'
    fileList = os.listdir(srcPath)
    prog = re.compile(r"\.h$|\.inl$")
    for fe in fileList :
        if prog.search(fe) :
            fullPathSrc = os.path.join(srcPath, fe)
            fullPathDst = os.path.join(destPath, fe)
            print '{:s} -> {:s}'.format(fullPathSrc, fullPathDst)
            shutil.copyfile(fullPathSrc, fullPathDst)

def copyLibraryFiles(dstDir, srcDir, srcToDst) :
    for tupleSrcAndDst in srcToDst :
        srcPath = os.path.join(srcDir, tupleSrcAndDst[0])
        fileList = os.listdir(srcPath)
        for file in fileList :
            fullSrcPath = os.path.join(srcPath, file)
            fullDstPath = os.path.join(os.path.join(dstDir, tupleSrcAndDst[1]), file)
            print '{:s} -> {:s}'.format(fullSrcPath, fullDstPath)
            shutil.copyfile(fullSrcPath, fullDstPath)

if __name__ == "__main__" :
    removeDirs(destinationDirectories[0][0])
    makeDirs(destinationDirectories)
    dstDir = os.path.join(os.path.join(destinationDirectories[0][0], 'include'), 'gfx')
    #
    copyHeaderFiles(sourceDirectory, dstDir)
    copyLibraryFiles('C:\\opt\\dev\\libs\\gfx_lib\\lib', sourceDirectoryLibs, 
                     [('Win32\\lib', 'X86'), ('X64\\lib', 'X64')])
