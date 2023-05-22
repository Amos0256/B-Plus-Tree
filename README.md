# B-Plus-Tree

### 姓名: 王信智&nbsp;&nbsp;&nbsp;&nbsp;學號: 409410116&nbsp;&nbsp;&nbsp;&nbsp;Email: 409410116@alum.ccu.edu.tw

#### 程式語言及作業系統版本 

* C++11
* Ubuntu 22.04.2 LTS

#### 基本功能

* [x] Initialize
* [x] Insert
* [x] Display
* [x] Quit

#### 說明如何操作你的程式

1. 打開 terminal 並且執行 makefile

   ```
   $ make
   ```

2. 執行 BPTree

   ```
   $ ./BPTree
   ```

3. 輸入 B+ 樹的 order

   ![image-20230522233831716](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230522233831716.png)

4. 選擇操作，按 1 執行 Insert，按 2 執行 Delete，按 3 執行 Display，按 4 離開

   ![image-20230522234011673](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230522234011673.png)

5. Insert，輸入要 insert 的數值，輸入後會顯示執行後的 B+ 樹

   ![image-20230523000242857](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230523000242857.png)

6. Delete，輸入要 insert 的數值，輸入後會顯示執行後的 B+ 樹

   ![image-20230523000452663](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230523000452663.png)

7. Display，顯示目前的 B+ 樹

   ![image-20230523000606166](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230523000606166.png)

8. Quit

   ![image-20230523000718137](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230523000718137.png)

#### 必列出非自行完成的部分

參考 [B-Plus-Tree](https://github.com/niteshkumartiwari/B-Plus-Tree) 架構，Insert 和 Delete 實作部分參考其 function 並且改寫成符合作業的版本。

#### Bonus

- [x] Delete

  

- [x] 遇到相同 data 的處理

  作法: 當 insert 遇到相同的 data 時，會跳出 *The key already in b+ tree!*，不會執行 insert 並且繼續下一次的輸入。

  ![image-20230523005336527](C:\Users\Amos\AppData\Roaming\Typora\typora-user-images\image-20230523005336527.png)
