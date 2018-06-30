# Operating System Project 3 Report

<p style="text-align:right">
Team #44: B05902073 張庭與、B05902095 涂家銘
</p>

## Baseline

### Code Tracing

1. Between **`mmap( )`** and **`filemap_fault( )`
 
    ![](https://imgur.com/L3PH7Mf.png
        =400x)

2. From **`filemap_fault( )`** to **`__do_page_cache_readhead( )`

    ![](https://imgur.com/ifE1bAG.png
        =400x)

### Implementation Detail

1. Between **`mmap( )`** and **`filemap_fault( )`**:
    - Assignment of **`filemap_fault( )`** to variables:
        - In most **`fs/[FS name]/file.c`**
            ```C
            const struct file_operations
                [FS name]_file_operations =
                {   // ...
                    // generic FS
                    .mmap = generic_file_mmap
                    
                    // some special FS, such as ext4
                    .mmap = [FS name]_file_mmap   
                    // ...
                }
            ```
        - Both **`file_mmap( )`** functions yields **`.fault = filemap_fault`**
    
    - Usage of value-assigned **`struct file_operations`**:
        - In function **`mmap_region( )`**, file **`mm/mmap.c`**
            ```C=1993
            vma -> vm_file = file;
            get_file (file);
            error = file -> f_op -> mmap(file, vma);
            ```
        - In funciton **`do_mmap_pgoff( )`**, file **`mm/map.c`**:
            ```C=1056
            return mmap_region (...);
            ```
        - In function **`do_mmap( )`**, file **`include/linux/mm.h`**:
            ```C=1158
            if(!(offset & ~PAGE_MASK))
                ret = do_mmap_pgoff (...);
            ```
        - In function **`ia32_do_mmap( )`**, file **`arch/ia64/ia32/sys_ia32.c`**:
            Either directly calls **`do_mmap( )`**,
            or calling **`emulate_mmap( )`**, **`mmap_subpage( )`** and finally **`do_mmap( )`**.
        - In file **`arch/ia64/ia32/sys_ia32.c`**:
            - In function **`sys32_mmap( )`**:
                ```C=918
                addr = ia32_do_mmap(...);
                ```
            - In function **`sys32_mmap2( )`**:
                ```C=939
                retval = ia32_do_mmap(...);
                ```
        - The two functions above are the equivalent of **`mmap( )`** and **`mmap2( )`**, respectively.
        Upwards tracing done.


2. Between **`filemap_fault( )`** and **`__do_page_cache_readhead( )`**:

    - **`filemap_fault( )`** : Handdling page fault. Decide whether to readahead and which kind (sync or async) of readahead to do.

    - **`do_async_readahead()`** and **```page_async_mmap_reqdahead( )```** : ++async readahead++ is used for reading ahead when there is not yet a page fault. For exmple , last read-ahead may set a PG_readahead flag at `start+size-async_size`, when offset hits the position, async readahead may be performed.
    - **`do_sync_readahead( )`** and **`page_sync_mmap_reqdahead( )`** : Different from async readahead, sync readahead usually called when page fault occured, so it should guarentee the first readed page can be used right away.
    
    - **`ondemand_readahead( )`** : Perform readahead algorithm. Calculate and update file_ra_state (read ahead state) by the offset of current memory access and call functions that read pages.

    - **`__do_page_cache_readahead( )`** : Actually read a chunk of memory from disk to memory. Pages are preserved before IO. 


### Readahead Algorithm Revision
:::info
Testing Environment:
VirtualBox on ASUS Notebook GL552VW
CPU: x2 Intel i7-6700HQ @ 2.6 (up to 3.5) GHz with HyperThreading
RAM: DDR4-2133 8192MB
Virtual Storage Device: 30.00GB, fixed size.
Physical Storage Device: 2.5" 1TB HDD (2 platters), 5400rpm with 8MB buffer.
:::

We found that **`VM_MAX_READAHEAD`** is the "hard upperbound" of readahead size, and it is also used to calculate initial readahead size and next readahead size (in **`get_init_ra_size( )`** and **`get_next_ra_size( )`**).
In theory, **`VM_MAX_READAHEAD`** should effect the speed of page fault handling: the larger, the faster (under a certain value that decrease memory efficiency too much).
Thus, we decided to either increase or decrease **`VM_MAX_READAHEAD`**, and test whether it meets our expectation.

#### Result of the test program:
$\text{Let }R_{max}$ denotes variable **`VM_MAX_READHEAD`**


|$R_{max}$| Test #1| Test #2| Test #3|Average Time|Major Page Fault|Minor Page Fault|
|:-------:|-------:|-------:|-------:|-----------:|---------------:|---------------:|
|$64$|$33.99$|$34.20$|$34.06$|$\approx34.1$|$4639$|$2156$|
|$128$<br>$\text{(Default)}$|$32.30$|$31.82$|$31.39$|$\approx31.8$|$4200$|$2596$|
|$128$* |$2.652$|$2.598$|$2.614$|$\approx2.62$|$4209$|$2584$|
|$256$|$26.76$|$26.42$|$26.66$|$\approx26.6$|$3507$|$3289$|
|$512$|$4.397$|$4.763$|$4.631$|$\approx4.60$|$353$|$6443$|
|$1024$|$3.028$|$3.051$|$3.029$|$\approx3.04$|$185$|$6611$|
|$2048$|$2.219$|$2.487$|$2.231$|$\approx2.31$|$95$|$6700$|


$\qquad$\* : Tested on computer with SSD in lab 204.


#### Conclusion
Apparently, it meets our expectation: the larger, the better. Memory capacity nowadays is large enough to accommodate readahead pages even if $R_{max}$ is as large as $2048$ MB. Even though, we should not use too many memory to readahead for its risk of using up memory capacity.

Noticeably, the time taken dropped dramatically from $R_{max}=256$ MB to $512$ MB, maybe relative to the working set size of the testing program.

Moreover, we see the difference between HDD and SSD. In spite of the similar number of page faults, SSD does not have seek time and rotation delay, thus having a much faster result than that of HDD.

In this test, we see the importance of a good readahead algorithm, and the balance between memory efficiency and IO speed.