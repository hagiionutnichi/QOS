pipeline {
  agent any
  stages {
    stage('UEFI') {
      steps {
        sh '''cd gnu-efi
make
make bootloader
cd ..'''
      }
    }

  }
}