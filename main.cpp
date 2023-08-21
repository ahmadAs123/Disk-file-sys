#include <iostream>
#include <vector>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include<map>

int io=0;
#define DISK_SIZE 256
using namespace std;
char *res;
char *res1;
char *res2;
char *res7;


string convert_func(char* a, int size)// function that convert char to string
{
    int iii=0;
    string re = "";
    while(iii<size) {
        re += a[iii];
        iii++;
    }

    return re;
}

char * substring (char a[],char b[]){ // function that give the substring between two arras

    string s1= convert_func(a,strlen(a));
    string s2=convert_func(b,strlen(b));
    string res5=s1.substr(s2.length(),(s1.length()-s2.length()));
    res7=new char[res5.length()+1];
    strcpy(res7, res5.c_str());
    res7[strlen(res7)]='\0';
    return res7;

}
// ============================================================================

void decToBinary(int n, char &c)
{
    // array to store binary number
    int binaryNum[8];

    // counter for binary array
    int i = 0;
    while (n > 0)
    {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
    {
        if (binaryNum[j] == 1)
            c = c | 1u << j;
    }
}



// ============================================================================

class FsFile {
    int file_size;
    int block_in_use;
    int index_block;
    int block_size;

public:
    FsFile(int _block_size) {
        file_size = 0;
        block_in_use = 0;
        block_size = _block_size;
        index_block = -1;

    }


    void setfile_size(int num) {
        this -> file_size = num;
    }

    int getfile_size() {
        return file_size;
    }

    void Set_IB(int num) {
        this -> index_block =num;
    }

    int Get_IB() {
        return this -> index_block;
    }




};

// ============================================================================

class FileDescriptor {
    string file_name;
    FsFile* fs_file;
    bool inUse;

public:

    FileDescriptor(string FileName, FsFile* fsi) {
        file_name = FileName;
        fs_file = fsi;
        inUse = true;
    }

    string getFileName() {
        return file_name;
    }


    FsFile* getfs() {
        return fs_file;
    }

    bool getIn_use() {
        return inUse;
    }

    void setIn_use(bool res) {
        this -> inUse=res;
    }

};

#define DISK_SIM_FILE "DISK_SIM_FILE.txt"

// ============================================================================

class fsDisk {
    FsFile * New_fs;
    FileDescriptor* New_File;
    FILE *sim_disk_fd;
    bool is_formated;
    int BitVectorSize;
    int *BitVector;
    int  max_file_size;
    int size_of_the_block;
    int sum_empty_blocks;
    vector<FileDescriptor*>MainDir;
    map<int ,FileDescriptor*>OpenFileDescriptors;

    // ------------------------------------------------------------------------
public:
    fsDisk() {
        sim_disk_fd = fopen(DISK_SIM_FILE , "r+");

        assert(sim_disk_fd);

        for (int i=0; i < DISK_SIZE ; i++) {
            int ret_val = fseek ( sim_disk_fd , i , SEEK_SET );
            ret_val = fwrite( "\0" ,  1 , 1, sim_disk_fd);
            assert(ret_val == 1);
        }

        fflush(sim_disk_fd);
        is_formated = false;
    }
    ~fsDisk(){// this is the distructor
        for (int b = 0; b < MainDir.size(); b++) {
            delete(MainDir[b] ->getfs());
            delete(MainDir[b]);
        }

        OpenFileDescriptors.clear();

        delete[]BitVector;
        BitVector=NULL;
//delete(New_File->getfs());
//delete(New_fs);
//delete[]res7;

        fclose(sim_disk_fd);
    }
    // ------------------------------------------------------------------------
    void listAll() {
        int i;

        for (i=0;i<MainDir.size();i++) {
            cout << "index: " << i << ": FileName: " << MainDir[i] -> getFileName()<<  " , isInUse: " <<MainDir[i] ->  getIn_use() << endl;

        }

        char bufy;
        cout << "Disk content: '";
        for (i = 0; i < DISK_SIZE; i++)
        {
            cout << "(";
            int ret_val = fseek(sim_disk_fd, i, SEEK_SET);
            ret_val = fread(&bufy, 1, 1, sim_disk_fd);
            cout << bufy;
            cout << ")";
        }
        cout << "'" << endl;

    }
    // ------------------------------------------------------------------------
    void fsFormat( int blockSize =4 ) {
//this formated the disk and rempve all the files
        BitVectorSize =int(DISK_SIZE/blockSize);
        sum_empty_blocks= BitVectorSize;
        size_of_the_block = blockSize;
        max_file_size=  pow(blockSize,2);
        for (int k =0; k<DISK_SIZE; k++) {  //fill disk with \0
            fseek(sim_disk_fd, k, SEEK_SET);
            fwrite("\0", 1, 1, sim_disk_fd);}

        BitVector = new int[BitVectorSize];
        for (int j = 0; j < BitVectorSize; j++) { // set all elemnts  the array with zeros
            BitVector[j] = 0;
        }
        is_formated = true;
        OpenFileDescriptors.clear();
        MainDir.clear();
        io=0;

    }
    // ------------------------------------------------------------------------
    int CreateFile(string fileName) { // creating new file and put in openfilediscriptor map
        int co=0;
        if(sim_disk_fd==NULL)
            return -1;
        if (is_formated==false)
            return -1;

        for(int xx=0;xx<BitVectorSize;xx++) {
            if (BitVector[xx] == 0) {
                co++;}}
        if(co==0)
            return -1;

        for(int uo=0;uo<MainDir.size();uo++){
            if(fileName==MainDir[uo]->getFileName()){

                return -1;
            }
        }

        New_fs = new FsFile(size_of_the_block);
        New_File =new FileDescriptor(fileName,New_fs);
        MainDir.push_back(New_File);
        for(int yc=0;yc<OpenFileDescriptors.size();yc++){
            if(OpenFileDescriptors[yc]==NULL){
                OpenFileDescriptors[yc]=New_File;
                return yc;
            }}
        OpenFileDescriptors[io]=New_File;
        io++;

        return io-1;

    }

    // ------------------------------------------------------------------------
    int OpenFile(string fileName) {// opening file that is closed

        if(sim_disk_fd==NULL)
            return -1;

        if (is_formated==false) {
            return -1;
        }




        for (int p = 0; p < MainDir.size(); p++) {
            if (MainDir[p] -> getFileName().compare(fileName)==0 && MainDir[p] -> getIn_use() == false) { // search if the file is already in maindir then only open it
                MainDir[p] -> setIn_use(true);
                for(int yy=0;yy<OpenFileDescriptors.size();yy++){
                    if(OpenFileDescriptors[yy]==NULL){
                        OpenFileDescriptors[yy]=MainDir[p];
                        return yy;
                    }
                }

                OpenFileDescriptors[io]=MainDir[p];
                io++;
                return io-1;

            }

        }

        return -1;
    }


    // ------------------------------------------------------------------------
    string CloseFile(int fd) { // closing the file

        if(sim_disk_fd==NULL)
            return "-1";
        string l;
        if (is_formated==false) //if isnt formated
            return "-1";

        if ( fd < 0)// there is no fd minous
            return "-1";

        if(fd>=OpenFileDescriptors.size())// the file wasnt open
            return"-1";
        if(OpenFileDescriptors[fd]==NULL)
            return "-1";

        OpenFileDescriptors[fd] -> setIn_use(false);//set the file in not use
        l=OpenFileDescriptors[fd] -> getFileName();
       // free(OpenFileDescriptors[fd]->getfs());
        OpenFileDescriptors[fd]=NULL;//delete it from the map for opend files

        return l;

    }
    // ------------------------------------------------------------------------
    int WriteToFile(int fd, char *buf, int len ) {
        int total_blocks = 0;  // writing on the disk
        int len_string;
        int free_tav;
        int real_num_block;
        int temp_file_size;
        int free_block_in_bitvector = 0;
        int tot_free_Fblocks;
        res=new char[strlen(buf)+1];
        res1=new char[strlen(buf)+1];
        res2=new char[strlen(buf)+1];
        int count=0;
        char a;
        char bab;
        if(size_of_the_block==0)
            return -1;
        BitVectorSize=(int)(DISK_SIZE/size_of_the_block); // the size of the bit victor

        if ( OpenFileDescriptors[fd]==NULL || is_formated == false || len < 0) { // if the disk didnt formated or the len is smaller than 0 or the file wasnt open
            delete[]res;
            delete[]res1;
            delete[]res2;
            return -1;
        }


        for (int a = 0; a <BitVectorSize ; a++) {     //count the free blocks
            if (BitVector[a] == 0)
                free_block_in_bitvector++;
        }


        if (free_block_in_bitvector == 0&&OpenFileDescriptors[fd]->getfs()->Get_IB()==-1) {
            delete[]res;
            delete[]res1;
            delete[]res2;
            //  free blocks in the  disk
            return -1;
        }
        if (free_block_in_bitvector == 1&&OpenFileDescriptors[fd]->getfs()->Get_IB()==-1) {
            delete[]res;
            delete[]res1;
            delete[]res2;
            //  free blocks in the  disk
            return -1;
        }


        tot_free_Fblocks =(int) ((max_file_size - OpenFileDescriptors[fd]->getfs()->getfile_size())/size_of_the_block);   //sum of the block for the file that we can use it



        if(OpenFileDescriptors[fd]->getfs()->Get_IB() == -1) {
            temp_file_size=0;                                    // the file was opend for the first time (nwe file)
            strcpy(res1,buf);
            res1[strlen(res1)]='\0';
            for(int x=0;x<BitVectorSize;x++) {
                if (BitVector[x] == 0) {
                    BitVector[x] = 1;
                    OpenFileDescriptors[fd]->getfs()->Set_IB(x);
                    break;
                }
            }

            len_string=len;
            while (tot_free_Fblocks > 0 && len_string > 0) {//while there is more free block
                if (len_string >=size_of_the_block) {
                    for (int g = 0;g<BitVectorSize ; g++) {
                        if (BitVector[g] == 0) {// we find free block
                            BitVector[g] = 1;
                            tot_free_Fblocks--;
                            fseek(sim_disk_fd, g * size_of_the_block, SEEK_SET);
                            strncpy(res, res1, size_of_the_block);
                            res[size_of_the_block]='\0';

                            fwrite(res, strlen(res), 1, sim_disk_fd);// write on the disk
                            len_string -= size_of_the_block;
                            strcpy(res2,substring(res1,res));   //the substring of the new string after we print part of it on the disk
                            strcpy(res1,res2);
                     //       res1[strlen(res1)]='\0';

                            fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block)+(size_of_the_block-tot_free_Fblocks-1), SEEK_SET);
                            temp_file_size+=strlen(res);
                            OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                            bab=(char)(g+48);
                            fwrite(&bab,1, 1, sim_disk_fd);
                            break;
                        }
                    }
                }
                else if (len_string < size_of_the_block) {//the len pf the string is smaller than the block size
                    for (int u = 0;u<BitVectorSize; u++) {
                        if (BitVector[u] == 0) {
                            BitVector[u] = 1;
                            fseek(sim_disk_fd, u * size_of_the_block, SEEK_SET);//going to the free block on the disk
                            strcpy(res,"");
                            strcpy(res, res1);
                            res[strlen(res)]='\0';
                            fwrite(res,strlen(res), 1, sim_disk_fd);   //print the string

                            len_string = 0;
                            temp_file_size+=strlen(res);
                            OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                            tot_free_Fblocks--;
                            fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block)+(size_of_the_block-tot_free_Fblocks-1), SEEK_SET);   //going to index block to write which block we wrote on it
                            bab=(char)(u+48);
                            fwrite(&bab,1, 1, sim_disk_fd);
                            break;
                        }
                    }
                }
                else;
            }
            delete[]res;
            delete[]res1;
            delete[]res2;
       //     delete[]res7;
            return 1;
        }






        if (OpenFileDescriptors[fd]->getfs()->Get_IB() != -1) { // we use this file before

            int u;
            temp_file_size=OpenFileDescriptors[fd]->getfs()->getfile_size();
            free_tav=0;                                                //we wrote on this file before
            fseek(sim_disk_fd, OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block, SEEK_SET);


            for ( u = 0; u < size_of_the_block; ) {

                fread(&a, 1, 1, sim_disk_fd);

                if (a == '\0'|| u==size_of_the_block-1 ) {    //check every block


                    fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block )+u - 1 ,SEEK_SET);
                    if(u==size_of_the_block-1&&a!='\0'){

                        fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block )+u  ,SEEK_SET);
                    }
                    fread(&a, 1, 1, sim_disk_fd);


                    real_num_block = (int)(a - 48);// convert to int

                    real_num_block *= size_of_the_block;

                    fseek(sim_disk_fd, real_num_block, SEEK_SET);

                    for (int o = 0; o < size_of_the_block; ) {
                        fread(&a, 1, 1, sim_disk_fd);
                        if (a == '\0')
                            free_tav++;
                        o++;
                        //  fseek(sim_disk_fd, real_num_block +o, SEEK_SET);

                    }


                    if (free_tav > 0 && len <= free_tav) {
                        strncpy(res, buf, len);
//                        res[strlen(res)]='\0';
                        fseek(sim_disk_fd, real_num_block + (size_of_the_block - free_tav), SEEK_SET);
                        fwrite(res, len, 1, sim_disk_fd);
                        temp_file_size+=len;
                        OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                        delete[]res;
                        delete[]res1;
                        delete[]res2;
                       // delete[]res7;
                        return 1;
                    }



                    if(free_tav==0){

                        strcpy(res1,buf);
                        res1[strlen(res1)]='\0';

                        len_string=len;

                        while (tot_free_Fblocks > 0 && len_string > 0) {//there is more blocks for the file
                            if (len_string >= size_of_the_block) {//if the string is bigeer than the block size
                                for (int f = 0; f < BitVectorSize; f++) {
                                    if (BitVector[f] == 0) {//we found free block
                                        BitVector[f] = 1;
                                        fseek(sim_disk_fd, f * size_of_the_block, SEEK_SET);    //going to this block to write on it
                                        strncpy(res, res1, size_of_the_block);
                                        res[size_of_the_block]='\0';
                                        fwrite(res, strlen(res), 1, sim_disk_fd);
                                        len_string -= size_of_the_block;
                                        strcpy(res2,substring(res1,res));
                                        strcpy(res1,res2);
                                      //  res1[strlen(res1)]='\0';
                                        temp_file_size+=strlen(res);
                                        OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                                        tot_free_Fblocks--;
                                        fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block)+(size_of_the_block-tot_free_Fblocks-1), SEEK_SET);
                                        bab=(char)(f+48);
                                        fwrite(&bab,1, 1, sim_disk_fd);
                                        break;
                                    }
                                }


                            } else if (len_string < size_of_the_block) {//the string is small than size the block
                                for (int w = 0; w < BitVectorSize; w++) {
                                    if (BitVector[w] == 0) {//we found free block
                                        BitVector[w] = 1;
                                        fseek(sim_disk_fd, w * size_of_the_block, SEEK_SET);//going to the block
                                        strcpy(res,"");
                                        strcpy(res, res1);
                                        res[strlen(res)]='\0';
                                        fwrite(res,strlen(res), 1, sim_disk_fd);  // write the string
                                        len_string = 0;
                                        tot_free_Fblocks--;
                                        temp_file_size+=strlen(res);
                                        OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                                        fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block)+(size_of_the_block-tot_free_Fblocks-1), SEEK_SET);
                                        bab=(char)(w+48);
                                        fwrite(&bab,1, 1, sim_disk_fd);
                                        break;
                                    }
                                }
                            }
                            else;
                        }
                        delete[]res;
                        delete[]res1;
                        delete[]res2;
                      //  delete[]res7;
                        return 1;

                    }




                    if (free_tav > 0 && len > free_tav ) {//if the empty tavem is more than 0 and the len is bigger than what we need in this block

                        strcpy(res1,buf);
                    //    res1[strlen(res1)]='\0';
                        strncpy(res, buf, free_tav);
                        res[free_tav]='\0';
                        fseek(sim_disk_fd, (real_num_block + (size_of_the_block - free_tav)), SEEK_SET);
                        fwrite(res, free_tav, 1, sim_disk_fd);
                        temp_file_size+=free_tav;
                        OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                        len_string = len - free_tav;
                        strcpy(res2,substring(res1,res));   //substring
                        strcpy(res1,"");
                        strcpy(res1,res2);
                   //     res1[strlen(res1)]='\0';



                        while (tot_free_Fblocks > 0 && len_string > 0) { // ther eis more free block

                            if (len_string >= size_of_the_block) {
                                for (int v = 0; v < BitVectorSize; v++) {
                                    if (BitVector[v] == 0) { //we found free block
                                        BitVector[v] = 1;
                                        //  cout<<v<<endl;
                                        fseek(sim_disk_fd, v * size_of_the_block, SEEK_SET);
                                        strncpy(res, res1, size_of_the_block);
                                        res[size_of_the_block]='\0';
                                        fwrite(res, strlen(res), 1, sim_disk_fd);
                                        len_string -= size_of_the_block;
                                        strcpy(res2,substring(res1,res));
                                        strcpy(res1,res2);
                                        res1[strlen(res1)]='\0';
                                        temp_file_size+=strlen(res);
                                        OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                                        tot_free_Fblocks=tot_free_Fblocks-1;
                                        fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block)+(size_of_the_block-tot_free_Fblocks-1), SEEK_SET);
                                        char abab=(char)(v+48);
                                        fwrite(&abab,1, 1, sim_disk_fd);
                                        break;
                                    }
                                }
                            }


                            else if (len_string < size_of_the_block) {

                                for (int q = 0; q < BitVectorSize; q++) {
                                    if (BitVector[q] == 0) {
                                        BitVector[q] = 1;
                                        fseek(sim_disk_fd, q * size_of_the_block, SEEK_SET);
                                        strcpy(res,"");
                                        strcpy(res, res1);
                                        res[len_string]='\0';
                                        fwrite(res,strlen(res), 1, sim_disk_fd);
                                        len_string = 0;
                                        temp_file_size+=strlen(res);
                                        OpenFileDescriptors[fd]->getfs()->setfile_size(temp_file_size);
                                        tot_free_Fblocks--;
                                        fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block)+(size_of_the_block-tot_free_Fblocks-1), SEEK_SET);
                                        char tbab=(char)(q+48);
                                        fwrite(&tbab,1, 1, sim_disk_fd);
                                        break;
                                    }
                                }
                            }
                            else;
                        }
                        delete[]res;
                        delete[]res1;
                        delete[]res2;
                        delete[]res7;
                        return 1;
                    }
                }
                u++;
                fseek(sim_disk_fd, OpenFileDescriptors[fd]->getfs()->Get_IB() * size_of_the_block + u, SEEK_SET);
            }

        }
        delete[]res;
        delete[]res1;
        delete[]res2;
        delete[]res7;
        return -1;
    }

    // ------------------------------------------------------------------------
    int DelFile( string FileName ) {

        int index_file = -2;
        int file_d;
        int q;
        char r;
        int place;


        if (sim_disk_fd == NULL || is_formated==false) {

            return -1;
        }


        for(int mkom=0;mkom<MainDir.size();mkom++) {
            //   cout <<"im here0"<<endl;// searching if the file is in the vector maindir
            if (MainDir[mkom] -> getFileName() == FileName) {
                index_file = mkom;
                break;}

        }



        if (index_file == -2) {           // we didnt find the file name in the vector maindir
            return -1;
        }

        if (MainDir[index_file] -> getIn_use()==true) {    // the file already open so we cant delete it!

            return -1;}







        if (MainDir[index_file] -> getfs() -> Get_IB() == -1) {
                                                                             // if the file wasnt in used befor then only delete it

            delete(MainDir[index_file] -> getfs());
            MainDir.erase(MainDir.begin()+index_file);
            return 1;
        }





        for ( q= 0; q < size_of_the_block; q++) {     //  set the bit victor 0 and fill all the blocks with \0
            fseek(sim_disk_fd, MainDir[index_file] -> getfs() -> Get_IB() * size_of_the_block + q, SEEK_SET);
            fread( & r, 1, 1, sim_disk_fd);
            if(r!='\0') {
                place = (int) (r - 48);
                BitVector[place] = 0;
                fseek(sim_disk_fd, place*size_of_the_block, SEEK_SET);
                for (int j = 0; j < size_of_the_block; ) {
                    fwrite("\0", 1, 1, sim_disk_fd);
                    j++;
                    fseek(sim_disk_fd, place*size_of_the_block+j, SEEK_SET);

                }
            }

        }
        int hit=MainDir[index_file] -> getfs() -> Get_IB();
        BitVector[hit] = 0;   // set the bit victor of index block with 0
        fseek(sim_disk_fd, hit * size_of_the_block , SEEK_SET); // going to the index block to fill it also with \0
        for (int z= 0; z< size_of_the_block; ) {
            fwrite("\0", 1, 1, sim_disk_fd);
            z++;
            fseek(sim_disk_fd, hit * size_of_the_block + z, SEEK_SET);
        }


        delete(MainDir[index_file] -> getfs());
        MainDir.erase(MainDir.begin()+index_file);
        return 1;

    }
    // ------------------------------------------------------------------------
    int ReadFromFile(int fd, char *buf, int len ) { //reading from file a specific num of chars
        int tot_read_block;
        int index_b;
        char y;
        char temp;
        int counter_t=0;
        int place=0;


        if( OpenFileDescriptors[fd]!=NULL&&(len>OpenFileDescriptors[fd]->getfs()->getfile_size()))// the len bigger than the file then take only what in the file
            len=OpenFileDescriptors[fd]->getfs()->getfile_size();

        tot_read_block= ceil((double)((double) len / (double) size_of_the_block)); // how much block the string take

        // cout<<tot_read_block<<endl;


        if (sim_disk_fd == NULL ) {
            buf[0] = '\0';   //if the fd wasnt opend
            return -1;
        }
        if(is_formated==false  ){
         // the file didnt formarted yet
            buf[0] = '\0';
            return -1;}


        if(OpenFileDescriptors[fd]==NULL){
            buf[0] = '\0';
            return -1;}

        if(len < 0){ // len is small than 0
            buf[0] = '\0';
            return -1;}

        if (len == 0) {
           // there is no string to take
            buf[0] = '\0';
            return 1;
        }


        for (int t = 0; t < tot_read_block; t++) {

          //loop on the blocks that the file use
            fseek(sim_disk_fd, OpenFileDescriptors[fd] -> getfs() -> Get_IB() * size_of_the_block + t, SEEK_SET);
            fread( & y, 1, 1, sim_disk_fd);
            index_b =(int)(y-48);
            fseek(sim_disk_fd, index_b * size_of_the_block, SEEK_SET);    //go to the disk by the number index_b


            for(int er=0;er<size_of_the_block;){
             //count how much wrote in specific block
                fread( & temp, 1, 1, sim_disk_fd);
                if(temp!='0')
                    counter_t++;
                er++;
                fseek(sim_disk_fd, index_b * size_of_the_block+er, SEEK_SET);

            }

            if(counter_t<size_of_the_block && len<=counter_t){ // the length is equal or smaller than what was wrotten
                fseek(sim_disk_fd, index_b * size_of_the_block, SEEK_SET);
                fread( & buf[place], len, 1, sim_disk_fd);
                place+=len;
                break;
            }

            if(counter_t<size_of_the_block && len>counter_t){// if the len was biger than what in the file then u must take fron another block
                fseek(sim_disk_fd, index_b * size_of_the_block, SEEK_SET);
                fread( & buf[place], len, 1, sim_disk_fd);
                place+=counter_t;
                len-=size_of_the_block;

            }
            if(counter_t==size_of_the_block&& len<=counter_t){// if the len is smaller than how much was wrotten on the block then take only how much the len
                fseek(sim_disk_fd, index_b * size_of_the_block, SEEK_SET);
                fread( & buf[place], len, 1, sim_disk_fd);
                place+=len;
                break;
            }
            if(counter_t==size_of_the_block&&len>counter_t)
            {      fseek(sim_disk_fd, index_b * size_of_the_block, SEEK_SET);
                fread( & buf[place], size_of_the_block, 1, sim_disk_fd);
                place+=size_of_the_block;
                len-=size_of_the_block;
            }
            counter_t=0;
        }
        buf[place] = '\0';
        return 1;

    }
};

int main() {
    int blockSize;
    int direct_entries;
    string fileName;
    char str_to_write[DISK_SIZE];
    char str_to_read[DISK_SIZE];
    int size_to_read;
    int _fd;

    fsDisk *fs = new fsDisk();
    int cmd_;
    while(1) {
        cin >> cmd_;
        switch (cmd_)
        {
            case 0:   // exit
                delete fs;
                exit(0);
                break;

            case 1:  // list-file
                fs->listAll();
                break;

            case 2:    // format
                cin >> blockSize;
                fs->fsFormat(blockSize);
                break;

            case 3:    // creat-file
                cin >> fileName;
                _fd = fs->CreateFile(fileName);
                cout << "CreateFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 4:  // open-file
                cin >> fileName;
                _fd = fs->OpenFile(fileName);
                cout << "OpenFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 5:  // close-file
                cin >> _fd;
                fileName = fs->CloseFile(_fd);
                cout << "CloseFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 6:   // write-file
                cin >> _fd;
                cin >> str_to_write;
                fs->WriteToFile( _fd , str_to_write , strlen(str_to_write) );
                break;

            case 7:    // read-file
                cin >> _fd;
                cin >> size_to_read ;
                fs->ReadFromFile( _fd , str_to_read , size_to_read );
                cout << "ReadFromFile: " << str_to_read << endl;
                break;

            case 8:   // delete file
                cin >> fileName;
                _fd = fs->DelFile(fileName);
                cout << "DeletedFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;
            default:
                break;
        }
    }

}