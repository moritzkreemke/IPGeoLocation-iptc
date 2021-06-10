#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <argp.h>
#include <regex.h>


//https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
const char *argp_program_version = "IP_Statistics Tool 1.0";
static char doc[] = "A Simple "; // Program documentation.
static char args_doc[] = "[IP-Address|FILE]"; // A description of the arguments we accept.
static struct argp_option options[] = {

        {"reference",    'r', "asn-country-ipv4.csv",      OPTION_ARG_OPTIONAL,  "Specify a reference IPV4 Database File" },
        {"follow",    'f', 0,      OPTION_ARG_OPTIONAL,  "If Input Stream defined it will update" },
        {"statistics",    's', 0,      OPTION_ARG_OPTIONAL,  "Prints statistics" },
        {"distinct",    'd', 0,      OPTION_ARG_OPTIONAL,  "Prints statistics" },
        { 0 }
}; //The options we understand.

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *args[2048];                /* IP-Adress or File */
    int follow;
    int statistics;
    int distinct;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'f':
            arguments->follow = 1;
            break;
        case 's':
            arguments->statistics = 1;
            break;
        case 'd':
            arguments->distinct = 1;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2)
                /* Too many arguments. */
                argp_usage (state);

            arguments->args[state->arg_num] = arg;

            break;

        case ARGP_KEY_END:
            if(isatty(STDIN_FILENO) == 1)
            {
                //if we are in terminal mode (no pipe or so)
                if(state->arg_num != 1)
                {
                    argp_usage(state);
                }
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc};


struct Entry
{
    long startRange;
    long endRange;
    char Country[2];
};

struct IPV4Country
{
    long ipAddress;
    char Country[2];
};

//https://pubs.opengroup.org/onlinepubs/7908799/xsh/regcomp.html
int IsIPV4 (const char* inetAddr)
{
    if(inetAddr == NULL)
        return 0;

    char* pattern = "^([0-9]{1,3}\\.){3}([0-9]{1,3})$"; //why not?
    int status;
    regex_t re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* report error */
    }
    status = regexec(&re, inetAddr, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return 0;      /* report error */
    }
    return 1;
}

long convertIPv4ToNumber (const char* inetAddr)
{
    //sometimes \n is part of the string, just ignore that
    char *tmpIP = malloc(strlen(inetAddr));
    strcpy((char *) tmpIP, inetAddr);
    tmpIP[strcspn(inetAddr, "\n")] = 0;

    struct in_addr inaddr;
    inet_pton(AF_INET, tmpIP, &inaddr);


    long number = ntohl(inaddr.s_addr);
    free(tmpIP);
    return number;
}

void convertNumberToIPv4 (char* result, long number)
{
    struct in_addr ip_addr;
    ip_addr.s_addr = htonl(number);

    strcpy(result,inet_ntoa(ip_addr));
}

int countlines(const char* filename)
{
    // count the number of lines in the file called filename
    FILE *fp = fopen(filename, "r");

    int ch=0;
    int lines=0;

    if (fp == NULL)
        return 0;

    lines++;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
            lines++;
    }
    fclose(fp);
    return lines;
}


void parseIPFile (struct Entry *arr, const char* filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0; //if 0 then line will be allcocated dynamicly?
    ssize_t read;

    fp = fopen(filename, "r");

    int counter = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        char *value;

        value = strtok(line, ",");
        arr[counter].startRange = convertIPv4ToNumber(value);
        for (int i = 0; value != NULL; i++) {
            //printf("Retrieved line of length %zu:\n", read);
            //printf("%s", line);
            value = strtok(NULL, ",");
            if (i == 0) {
                arr[counter].endRange = convertIPv4ToNumber(value);
            } else if (i == 1) {
                for (int i = 0; i < 2; i++) {
                    arr[counter].Country[i] = value[i];
                }
            }
        }
        counter++;
    }
    fclose(fp);
    if (line)
        free(line);
}

void getCountryFromIP (char result[2], const char *ipAddr, const struct Entry *arr, const int length)
{
    long ipNum = convertIPv4ToNumber(ipAddr);
    for(int i = 0; i < length; i++)
    {
        if(arr[i].startRange <= ipNum && arr[i].endRange >= ipNum)
        {
            strcpy(result, arr[i].Country);
        }
    }
}



struct IPV4Country* ReadIPV4AddressesFromStream (int* out_length, FILE* fp, int following, int IsNewStream)
{

    struct IPV4Country* arr = NULL;

    char *line = NULL;
    size_t len = 0; //if 0 then line will be allcocated dynamicly?
    ssize_t read;

    static long startPos = 0;
    if(following == 1 && startPos > 0 && IsNewStream == 1)
    {
        for(long i = 0; i < startPos; i++)
        {
            getc(fp);
        }
    }

    int counter = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        startPos += read;
        line[strcspn(line, "\n")] = 0;
        char* value;
        value = strtok(line, " ");
        while (value != NULL)
        {
            if(IsIPV4(value))
            {
                //distinct check?


                arr = realloc(arr, (counter + 1) * sizeof (struct IPV4Country));

                arr[counter].ipAddress = convertIPv4ToNumber(value);
                counter++;
                //add IPV4
            }
            value = strtok(NULL, " ");
        }
        if(following)
            break;
    }
    if (line)
        free(line);
    //counter--;
    *out_length = counter;
    return arr;
}

struct IPV4Country* GetIPV4FromTextFile (int* out_length, const char *filename, int following)
{
    FILE *fp;
    fp = fopen(filename, "r");
    struct IPV4Country* result =  ReadIPV4AddressesFromStream(out_length, fp, following, 1);
    fclose(fp);
    return result;
}

struct IPV4Country* GetIPV4FromStdIn (int* out_length, int following)
{
    FILE* fp;
    fp = stdin;
    return ReadIPV4AddressesFromStream(out_length, fp, following, 0);

}

void ResolveArrayOfIpAddresses (struct IPV4Country* result, const int length_result, const struct Entry* arr, const int length_arr)
{
    for(int i = 0; i < length_result; i++)
    {
        char country[2];
        char ipAddr[15];
        convertNumberToIPv4(ipAddr,result[i].ipAddress);
        getCountryFromIP(country,ipAddr , arr, length_arr);
        strcpy(result[i].Country, country);
    }
}

void printResult (const struct IPV4Country* arr, int length, int distinct)
{
    static long* ipNumbers;
    static int ipNumbersLength = 0;

    for(int i = 0; i < length; i++)
    {
        int alreadyContainsIP = 0;
        if(distinct)
        {
            //check if ip is pressend, if so break; otherwiese add
            for(int k = 0; k < ipNumbersLength; k++)
            {
                if(ipNumbers[k] == arr[i].ipAddress)
                {
                    alreadyContainsIP = 1;
                    break;
                }
            }
            if(alreadyContainsIP == 0)
            {
                ipNumbersLength++;
                ipNumbers = realloc(ipNumbers, ipNumbersLength * sizeof (ipNumbers));
                ipNumbers[ipNumbersLength - 1] = arr[i].ipAddress;
            }
        }
        if(distinct == 0 || (distinct == 1 && alreadyContainsIP == 0))
        {
            char ipAddr[15];
            convertNumberToIPv4(ipAddr, arr[i].ipAddress);
            printf("The IP %s comes from %s\n", ipAddr, arr[i].Country);
        }

    }
}

int compare (const void *a, const void *b)
{
    //what is void? excatly?
    struct IPV4Country* v1 = (struct IPV4Country*)a;
    struct IPV4Country* v2 = (struct IPV4Country*)b;


    return (v2->ipAddress - v1->ipAddress); //long to int in uncool but this number shouldn't be to high anyway

}

char * my_strcatt(char *s1, const char *s2)
{
    const size_t a = strlen(s1);
    const size_t b = strlen(s2);
    const size_t size_ab = a + b + 1;

    s1 = realloc(s1, size_ab);

    memcpy(s1 + a, s2, b + 1);

    return s1;
}


void printStatistics (const struct IPV4Country* arr, int arr_length, int distinct)
{
    //IPV4Country will be reused to store Country + Times it found
    static struct IPV4Country* stats; //no free, memory leak?
    static int length = 0;

    static long* ipNumbers;
    static int ipNumbersLength = 0;

    for(int i = 0; i < arr_length; i++)
    {
        _Bool contains = 0;
        for(int k = 0; k < length; k++)
        {
            if(strcmp(stats[k].Country, arr[i].Country) == 0)
            {
                //if distinct ?
                int ip_alreadyCounted = 0;
                if(distinct)
                {
                    for(int l = 0; l < ipNumbersLength; l++)
                    {
                        if(ipNumbers[l] == arr[i].ipAddress){
                            ip_alreadyCounted = 1;
                            break;
                        }
                    }
                    if(ip_alreadyCounted == 0)
                    {
                        ipNumbersLength++;
                        ipNumbers = realloc(ipNumbers, ipNumbersLength * sizeof (ipNumbers));
                        ipNumbers[length - 1] = arr[i].ipAddress;
                    }
                }
                if(distinct == 1 && ip_alreadyCounted == 1)
                {
                    contains = 1;
                } else
                {
                    stats[k].ipAddress++;
                    contains = 1;
                    break;
                }
            }
        }
        if(contains == 0)
        {
            length++;
            stats = realloc(stats, length * sizeof (struct IPV4Country));
            strcpy(stats[length - 1].Country, arr[i].Country);
            stats[length -1].ipAddress = 1;

            if(distinct)
            {
                ipNumbersLength++;
                ipNumbers = realloc(ipNumbers, ipNumbersLength * sizeof (ipNumbers));
                ipNumbers[length - 1] = arr[i].ipAddress;
            }

        }
    }
    //sort it :)
    if(length > 1)
        qsort(stats, length, sizeof (struct IPV4Country), compare);

    switch (length) {
        case 0:
            printf("\rnone");
            break;
        case 1:
            printf("\r%s:%ld", stats[0].Country, stats[0].ipAddress);
            break;
        case 2:
            printf("\r%s:%ld; %s:%ld", stats[0].Country, stats[0].ipAddress, stats[1].Country, stats[1].ipAddress);
            break;
        case 3:
            printf("\r%s:%ld; %s:%ld; %s:%ld", stats[0].Country, stats[0].ipAddress
                   ,stats[1].Country, stats[1].ipAddress
                   ,stats[2].Country, stats[2].ipAddress);
            break;
        case 4:
            printf("\r%s:%ld; %s:%ld; %s:%ld; %s:%ld", stats[0].Country, stats[0].ipAddress
                    ,stats[1].Country, stats[1].ipAddress
                    ,stats[2].Country, stats[2].ipAddress
                    ,stats[3].Country, stats[3].ipAddress);
            break;
        default:
            printf("\r%s:%ld; %s:%ld; %s:%ld; %s:%ld; %s:%ld", stats[0].Country, stats[0].ipAddress
                    ,stats[1].Country, stats[1].ipAddress
                    ,stats[2].Country, stats[2].ipAddress
                    ,stats[3].Country, stats[3].ipAddress
                    ,stats[4].Country, stats[4].ipAddress);
            break;
    }
    fflush(stdout);
}

#if !defined(TEST_RUN)
#define FILE_NAME "resources/asn-country-ipv4.csv"
int main(int argc,char *argv[])
{

    struct arguments arguments;

    /* Default values. */
    arguments.follow = 0;
    arguments.statistics = 0;
    arguments.distinct = 0;

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);


    struct Entry *arr;
    int lines = countlines(FILE_NAME);
    arr = calloc(lines, sizeof (struct Entry));
    parseIPFile(arr, FILE_NAME);

    struct IPV4Country* resultArr;
    int length;

    if(IsIPV4(arguments.args[0]))
    {
        char result[2];
        getCountryFromIP(result, argv[1], arr, lines);
        if(strcmp(result, "") == 0)
            strcpy(result, "??");
        printf("Your IP-Address is from: %s" , result);
    } else if(access(arguments.args[0], R_OK) == 0)
    {
        //its a file


        do{
            resultArr = GetIPV4FromTextFile(&length, arguments.args[0], arguments.follow);
            ResolveArrayOfIpAddresses(resultArr, length, arr, lines);
            //printf("%d", length);
            if(arguments.statistics == 0)
                printResult(resultArr, length, arguments.distinct);
            else
                printStatistics(resultArr, length, arguments.distinct);

            free(resultArr);
        } while (arguments.follow == 1);


        //read it
    } else if(isatty(STDIN_FILENO) == 0)
    {
        //if input is a pipe or text file
        do{
            resultArr = GetIPV4FromStdIn(&length, arguments.follow);
            ResolveArrayOfIpAddresses(resultArr, length, arr, lines);
            //printf("%d", length);
            if(arguments.statistics == 0)
                printResult(resultArr, length, arguments.distinct);
            else
                printStatistics(resultArr, length, arguments.distinct);
            free(resultArr);
        } while (arguments.follow == 1);

    }
    printf("\n");
    free(arr);
    exit(0);
}
#endif
