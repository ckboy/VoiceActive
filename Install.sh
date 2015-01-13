echo "Quick Install script for Voice Active 2.0"
echo "Compiling..."
make 
echo "copying neccessary files..."
sudo cp tts /usr/bin/
sudo cp VoiceActive /usr/bin
sudo cp listen.sh /usr/bin
echo "done."

