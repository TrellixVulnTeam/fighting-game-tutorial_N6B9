# This script is used to install local dependencies stored in Git LFS as archive
# It creates necessary folder structure, unpacks archive and copies files according to folder structure

import platform
import os.path
import shutil
import zipfile
import argparse
import tarfile


# Declare script arguments
parser = argparse.ArgumentParser(description='Install project dependencies.')
parser.add_argument("--platform", help="Override platform name to install dependencies. Useful for cross compilation.")
args = parser.parse_args()


# Variants for different archive types extraction
def extract_zip(file_path, extract_path):
    with zipfile.ZipFile(file_path, 'r') as zip_ref:
        zip_ref.extractall(extract_path)
        print('Unzipped into %s' % extract_path)


def extract_targz(file_path, extract_path):
    with tarfile.open(file_path, "r:gz") as targz_file:
        targz_file.extractall(path=extract_path)
        print('Extracted into %s' % extract_path)


platform_configs = {
    'windows': {
        'sharedLibExtension': '.dll',
        'archivePath': os.path.join('external', 'windows.zip'),
        'lib_types': ['.lib'],
        'binary_types': ['.dll'],
        'extractor': extract_zip,
    },
    'linux': {
        'sharedLibExtension': '.so',
        'archivePath': os.path.join('external', 'linux.tar.gz'),
        'lib_types': ['.a', '.so'],
        'binary_types': ['.so'],
        'extractor': extract_targz,
    },
    'darwin': {
        'sharedLibExtension': '.so',
        'archivePath': os.path.join('external', 'mac.tar.gz'),
        'lib_types': ['.a', '.so'],
        'binary_types': ['.so'],
    },
}


def ensure_directory_exists(directory_path):
    if not os.path.isdir(directory_path):
        print("%s directory doesn't exists, creating..." % directory_path)
        os.mkdir(directory_path)


def create_folders_structure():
    ensure_directory_exists('bin')
    ensure_directory_exists('libs')
    ensure_directory_exists(os.path.join('libs', 'lib'))


def remove_dir_tree(path):
    if os.path.isdir(path):
        shutil.rmtree(path)


# Create folder structure
create_folders_structure()

# Detect platform name
platformName = args.platform or platform.system().lower()
print("Platform: %s" % platformName)

# Check if platform was found in config
if platformName not in platform_configs:
    print('Unsupported platform? "%s"' % platformName)
    exit(1)

platform_config = platform_configs[platformName]

# Unzip archive
print('Unzipping archive...')
if os.path.isfile(platform_config['archivePath']):
    extract_dir = os.path.join('external', 'extracted_temp')

    platform_config['extractor'](platform_config['archivePath'], extract_dir)

    # Move include folder as is
    print('Copying includes...')
    remove_dir_tree(os.path.join('libs', 'include'))
    shutil.move(os.path.join(extract_dir, 'include'), 'libs')
    print('Done.')

    # Copy link libraries to lib folder
    print('Copying library files and binaries...')
    for f in os.listdir(os.path.join(extract_dir, 'lib')):
        for libType in platform_config['lib_types']:
            if f.endswith(libType):
                shutil.copyfile(os.path.join(extract_dir, 'lib', f), os.path.join('libs', 'lib', f))
                print('Copied "%s" to "%s"' % (os.path.join(extract_dir, 'lib', f), os.path.join('libs', 'lib', f)))
                break

        for binType in platform_config['binary_types']:
            if f.endswith(binType):
                shutil.copyfile(os.path.join(extract_dir, 'lib', f), os.path.join('bin', f))
                print('Copied "%s" to "%s"' % (os.path.join(extract_dir, 'lib', f), os.path.join('bin', f)))
                break
    print('Done.')

    # Remove temp folder with extracted content
    print('Removing temp dir %s...' % extract_dir)
    remove_dir_tree(extract_dir)
else:
    print("Archive file for given platform not found!")
