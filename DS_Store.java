import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.charset.StandardCharsets;
import java.io.BufferedWriter;
import java.io.FileWriter;

class TrieNode {
    AVLNode avlRoot;
    boolean isEnd;

    TrieNode() {
        this.avlRoot = null;
        this.isEnd = false;
    }
}

class AVLNode {
    int data;
    AVLNode left, right;
    TrieNode triNode;
    int height;

    AVLNode(int data) {
        this.data = data;
        this.left = this.right = null;
        this.triNode = new TrieNode();
        this.height = 1;
    }
}

class AVLInsertResult {
    AVLNode root;
    AVLNode endNode;

    AVLInsertResult(AVLNode root, AVLNode endNode) {
        this.root = root;
        this.endNode = endNode;
    }
}

public class DS_Store {

    private static int height(AVLNode root) {
        return (root != null) ? root.height : 0;
    }

    private static int max(int a, int b) {
        return (a > b) ? a : b;
    }

    private static int getBalance(AVLNode root) {
        return (root != null) ? height(root.left) - height(root.right) : 0;
    }

    private static AVLNode rightRotate(AVLNode y) {
        AVLNode x = y.left;
        AVLNode T2 = x.right;
        x.right = y;
        y.left = T2;
        y.height = 1 + max(height(y.left), height(y.right));
        x.height = 1 + max(height(x.left), height(x.right));
        return x;
    }

    private static AVLNode leftRotate(AVLNode x) {
        AVLNode y = x.right;
        AVLNode T2 = y.left;
        y.left = x;
        x.right = T2;
        x.height = 1 + max(height(x.left), height(x.right));
        y.height = 1 + max(height(y.left), height(y.right));
        return y;
    }

    private static AVLInsertResult insertNode(AVLNode root, int data) {
        if (root == null) {
            AVLNode newNode = new AVLNode(data);
            return new AVLInsertResult(newNode, newNode);
        }

        AVLInsertResult result;
        if (data < root.data) {
            result = insertNode(root.left, data);
            root.left = result.root;
        } else if (data > root.data) {
            result = insertNode(root.right, data);
            root.right = result.root;
        } else {
            return new AVLInsertResult(root, root);
        }

        root.height = 1 + max(height(root.left), height(root.right));
        int balance = getBalance(root);

        if (balance > 1 && data < root.left.data)
            return new AVLInsertResult(rightRotate(root), result.endNode);
        if (balance < -1 && data > root.right.data)
            return new AVLInsertResult(leftRotate(root), result.endNode);
        if (balance > 1 && data > root.left.data) {
            root.left = leftRotate(root.left);
            return new AVLInsertResult(rightRotate(root), result.endNode);
        }
        if (balance < -1 && data < root.right.data) {
            root.right = rightRotate(root.right);
            return new AVLInsertResult(leftRotate(root), result.endNode);
        }

        return new AVLInsertResult(root, result.endNode);
    }

    private static TrieNode insertTrieNode(TrieNode root, int[] codepoints) {
        TrieNode current = root;
        for (int cp : codepoints) {
            AVLInsertResult res = insertNode(current.avlRoot, cp);
            current.avlRoot = res.root;
            current = res.endNode.triNode;
        }
        current.isEnd = true;
        return root;
    }

    private static int[] decodeUTF8(String s) {
        return s.codePoints().toArray(); // Easy in Java
    }

    private static void serializeAVLPreOrder(AVLNode root, BufferedWriter bw) throws IOException {
        if (root == null) {
            bw.write("X ");
            return;
        }
        bw.write("N " + root.data + " ");
        serializeTriePreOrder(root.triNode, bw);
        serializeAVLPreOrder(root.left, bw);
        serializeAVLPreOrder(root.right, bw);
    }

    private static void serializeTriePreOrder(TrieNode t, BufferedWriter bw) throws IOException {
        if (t == null) {
            bw.write("X ");
            return;
        }
        bw.write("T " + (t.isEnd ? 1 : 0) + " ");
        serializeAVLPreOrder(t.avlRoot, bw);
    }

    private static void serializeAVLInOrder(AVLNode root, BufferedWriter bw) throws IOException {
        if (root == null) {
            bw.write("X ");
            return;
        }
        serializeAVLInOrder(root.left, bw);
        bw.write("N " + root.data + " ");
        serializeTrieInOrder(root.triNode, bw);
        serializeAVLInOrder(root.right, bw);
    }

    private static void serializeTrieInOrder(TrieNode t, BufferedWriter bw) throws IOException {
        if (t == null) {
            bw.write("X ");
            return;
        }
        bw.write("T " + (t.isEnd ? 1 : 0) + " ");
        serializeAVLInOrder(t.avlRoot, bw);
    }

    public static void main(String[] args) {
        String fileContent = readFile("sample.txt");
        if (fileContent == null) {
            System.err.println("Error reading sample.txt");
            return;
        }

        TrieNode trieRoot = new TrieNode();
        String[] tokens = fileContent.split("\\s+");

        for (String token : tokens) {
            int[] codepoints = decodeUTF8(token);
            trieRoot = insertTrieNode(trieRoot, codepoints);
        }

        try (BufferedWriter bwPre = new BufferedWriter(new FileWriter("preorder.txt"));
                BufferedWriter bwIn = new BufferedWriter(new FileWriter("inorder.txt"))) {
            serializeTriePreOrder(trieRoot, bwPre);
            serializeTrieInOrder(trieRoot, bwIn);
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Trie serialized into preorder.txt and inorder.txt successfully.");
    }

    private static String readFile(String filename) {
        try {
            byte[] allBytes = Files.readAllBytes(new File(filename).toPath());
            return new String(allBytes, StandardCharsets.UTF_8);
        } catch (IOException e) {
            System.err.println("Failed to read file: " + filename);
            return null;
        }
    }
}
