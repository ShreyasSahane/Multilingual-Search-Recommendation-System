import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.*;

public class DS_Retrive {
    static class TrieNode {
        AVLNode avlRoot;
        boolean isEnd;
        TrieNode() { this.avlRoot = null; this.isEnd = false; }
    }

    static class AVLNode {
        int data, height;
        AVLNode left, right;
        TrieNode triNode;
        AVLNode(int data) {
            this.data = data;
            this.height = 1;
            this.left = this.right = null;
            this.triNode = new TrieNode();
        }
    }

    static class Mapping {
        String eng;
        int[] codes;
        boolean dependent;
        Mapping(String eng, int[] codes, boolean dependent) {
            this.eng = eng;
            this.codes = codes;
            this.dependent = dependent;
        }
    }
    public static final Mapping[] mappings = new Mapping[] {
        new Mapping("a",    new int[]{0x0905}, false),
        new Mapping("A",    new int[]{0x0906}, false),
        new Mapping("aa",   new int[]{0x0906}, false),
        new Mapping("iI",   new int[]{0x0907}, false),
        new Mapping("II",   new int[]{0x0908}, false),
        new Mapping("iiI",  new int[]{0x0908}, false),
        new Mapping("ee",   new int[]{0x0908}, false),
        new Mapping("u",    new int[]{0x0909}, false),
        new Mapping("U",    new int[]{0x090A}, false),
        new Mapping("uu",   new int[]{0x090A}, false),
        new Mapping("RRi",  new int[]{0x090B}, false),
        new Mapping("R^i",  new int[]{0x090B}, false),
        new Mapping("RRI",  new int[]{0x0960}, false),
        new Mapping("R^I",  new int[]{0x0960}, false),
        new Mapping("LLi",  new int[]{0x090C}, false),
        new Mapping("L^i",  new int[]{0x090C}, false),
        new Mapping("LLI",  new int[]{0x0961}, false),
        new Mapping("L^I",  new int[]{0x0961}, false),
        new Mapping("^e",   new int[]{0x090E}, false),
        new Mapping("e",    new int[]{0x090F}, false),
        new Mapping("E",    new int[]{0x090F}, false),
        new Mapping("ae",   new int[]{0x090D}, false),
        new Mapping("aE",   new int[]{0x090D}, false),
        new Mapping("ai",   new int[]{0x0910}, false),
        new Mapping("^o",   new int[]{0x0912}, false),
        new Mapping("o",    new int[]{0x0913}, false),
        new Mapping("O",    new int[]{0x0913}, false),
        new Mapping("au",   new int[]{0x0914}, false),
        new Mapping("aU",   new int[]{0x0972}, false),
        new Mapping("AU",   new int[]{0x0911}, false),
        new Mapping(".a",   new int[]{0x093D}, false),
        new Mapping("OM",   new int[]{0x0950}, false),
        new Mapping("AUM",  new int[]{0x0950}, false),
    
        new Mapping("ka",   new int[]{0x0915}, false),
        new Mapping("k",    new int[]{0x0915}, false),
        new Mapping("kha",  new int[]{0x0916}, false),
        new Mapping("kh",   new int[]{0x0916}, false),
        new Mapping("ga",   new int[]{0x0917}, false),
        new Mapping("g",    new int[]{0x0917}, false),
        new Mapping("gha",  new int[]{0x0918}, false),
        new Mapping("gh",   new int[]{0x0918}, false),
        new Mapping("~Na",  new int[]{0x0919}, false),
        new Mapping("~N",   new int[]{0x0919}, false),
    
        new Mapping("cha",  new int[]{0x091A}, false),
        new Mapping("ch",   new int[]{0x091A}, false),
        new Mapping("Cha",  new int[]{0x091B}, false),
        new Mapping("Ch",   new int[]{0x091B}, false),
        new Mapping("ja",   new int[]{0x091C}, false),
        new Mapping("j",    new int[]{0x091C}, false),
        new Mapping("jha",  new int[]{0x091D}, false),
        new Mapping("jh",   new int[]{0x091D}, false),
        new Mapping("~na",  new int[]{0x091E}, false),
        new Mapping("~n",   new int[]{0x091E}, false),
    
        new Mapping("Ta",   new int[]{0x091F}, false),
        new Mapping("T",    new int[]{0x091F}, false),
        new Mapping("Tha",  new int[]{0x0920}, false),
        new Mapping("Th",   new int[]{0x0920}, false),
        new Mapping("Da",   new int[]{0x0921}, false),
        new Mapping("D",    new int[]{0x0921}, false),
        new Mapping("Dha",  new int[]{0x0922}, false),
        new Mapping("Dh",   new int[]{0x0922}, false),
        new Mapping("Na",   new int[]{0x0923}, false),
        new Mapping("N",    new int[]{0x0923}, false),
    
        new Mapping("ta",   new int[]{0x0924}, false),
        new Mapping("t",    new int[]{0x0924}, false),
        new Mapping("tha",  new int[]{0x0925}, false),
        new Mapping("th",   new int[]{0x0925}, false),
        new Mapping("da",   new int[]{0x0926}, false),
        new Mapping("d",    new int[]{0x0926}, false),
        new Mapping("dha",  new int[]{0x0927}, false),
        new Mapping("dh",   new int[]{0x0927}, false),
        new Mapping("na",   new int[]{0x0928}, false),
        new Mapping("n",    new int[]{0x0928}, false),
    
        new Mapping("pa",   new int[]{0x092A}, false),
        new Mapping("p",    new int[]{0x092A}, false),
        new Mapping("pha",  new int[]{0x092B}, false),
        new Mapping("ph",   new int[]{0x092B}, false),
        new Mapping("ba",   new int[]{0x092C}, false),
        new Mapping("b",    new int[]{0x092C}, false),
        new Mapping("bha",  new int[]{0x092D}, false),
        new Mapping("bh",   new int[]{0x092D}, false),
        new Mapping("ma",   new int[]{0x092E}, false),
        new Mapping("m",    new int[]{0x092E}, false),
    
        new Mapping("ya",   new int[]{0x092F}, false),
        new Mapping("y",    new int[]{0x092F}, false),
        new Mapping("ra",   new int[]{0x0930}, false),
        new Mapping("r",    new int[]{0x0930}, false),
        new Mapping("la",   new int[]{0x0932}, false),
        new Mapping("l",    new int[]{0x0932}, false),
        new Mapping("va",   new int[]{0x0935}, false),
        new Mapping("v",    new int[]{0x0935}, false),
        new Mapping("sha",  new int[]{0x0936}, false),
        new Mapping("sh",   new int[]{0x0936}, false),
        new Mapping("Sha",  new int[]{0x0937}, false),
        new Mapping("Sh",   new int[]{0x0937}, false),
        new Mapping("sa",   new int[]{0x0938}, false),
        new Mapping("s",    new int[]{0x0938}, false),
        new Mapping("ha",   new int[]{0x0939}, false),
        new Mapping("h",    new int[]{0x0939}, false),
        new Mapping("La",   new int[]{0x0933}, false),
        new Mapping("L",    new int[]{0x0933}, false),
    
        new Mapping("qa",   new int[]{0x0958}, false),
        new Mapping("q",    new int[]{0x0958}, false),
        new Mapping("Ka",   new int[]{0x0959}, false),
        new Mapping("K",    new int[]{0x0959}, false),
        new Mapping("Ga",   new int[]{0x095A}, false),
        new Mapping("G",    new int[]{0x095A}, false),
        new Mapping("za",   new int[]{0x095B}, false),
        new Mapping("z",    new int[]{0x095B}, false),
        new Mapping("fa",   new int[]{0x095C}, false),
        new Mapping("f",    new int[]{0x095C}, false),
        new Mapping(".Da",  new int[]{0x095D}, false),
        new Mapping(".D",   new int[]{0x095D}, false),
        new Mapping("Ra",   new int[]{0x095D}, false),
        new Mapping("R",    new int[]{0x095D}, false),
        new Mapping(".Dha", new int[]{0x095E}, false),
        new Mapping(".Dh",  new int[]{0x095E}, false),
        new Mapping("Rha",  new int[]{0x095E}, false),
        new Mapping("Rh",   new int[]{0x095E}, false),
        new Mapping("wa",   new int[]{0x0935,0x093C}, false),
        new Mapping("w",    new int[]{0x0935,0x093C}, false),
    
        new Mapping("kSa",  new int[]{0x0915,0x094D,0x0937}, false),
        new Mapping("kSha", new int[]{0x0915,0x094D,0x0937}, false),
        new Mapping("xa",   new int[]{0x0915,0x094D,0x0937}, false),
        new Mapping("tra",  new int[]{0x0924,0x094D,0x0930}, false),
        new Mapping("tr",   new int[]{0x0924,0x094D,0x0930}, false),
        new Mapping("GYa",  new int[]{0x091C,0x094D,0x091E}, false),
        new Mapping("j~na", new int[]{0x091C,0x094D,0x091E}, false),
        new Mapping("shra", new int[]{0x0936,0x094D,0x0930}, false),
    
        new Mapping("i",    new int[]{0x093F}, true),
        new Mapping("I",    new int[]{0x0940}, true),
        new Mapping("ii",   new int[]{0x0940}, true),
        new Mapping("ee",   new int[]{0x0940}, true),
        new Mapping("u",    new int[]{0x0941}, true),
        new Mapping("U",    new int[]{0x0942}, true),
        new Mapping("uu",   new int[]{0x0942}, true),
        new Mapping("RRi",  new int[]{0x0943}, true),
        new Mapping("R^i",  new int[]{0x0943}, true),
        new Mapping("LLi",  new int[]{0x0962}, true),
        new Mapping("L^i",  new int[]{0x0962}, true),
        new Mapping("e",    new int[]{0x0947}, true),
        new Mapping("E",    new int[]{0x0947}, true),
        new Mapping("ai",   new int[]{0x0948}, true),
        new Mapping("o",    new int[]{0x094B}, true),
        new Mapping("O",    new int[]{0x094B}, true),
        new Mapping("au",   new int[]{0x094C}, true),
    
        new Mapping("M",    new int[]{0x0902}, false),
        new Mapping(".m",   new int[]{0x0902}, false),
        new Mapping(".n",   new int[]{0x0902}, false),
        new Mapping("H",    new int[]{0x0903}, false),
        new Mapping(".N",   new int[]{0x0901}, false),
        new Mapping(".h",   new int[]{0x094D}, false),
        new Mapping(".c",   new int[]{0x0945}, false)
    };
    
    private static int height(AVLNode n) { return n == null ? 0 : n.height; }
    private static int max(int a, int b) { return a > b ? a : b; }

    @SuppressWarnings("unused")
    private static AVLNode rightRotate(AVLNode y) {
        AVLNode x = y.left, T2 = x.right;
        x.right = y;  y.left = T2;
        y.height = 1 + max(height(y.left), height(y.right));
        x.height = 1 + max(height(x.left), height(x.right));
        return x;
    }
    @SuppressWarnings("unused")
    private static AVLNode leftRotate(AVLNode x) {
        AVLNode y = x.right, T2 = y.left;
        y.left = x;   x.right = T2;
        x.height = 1 + max(height(x.left), height(x.right));
        y.height = 1 + max(height(y.left), height(y.right));
        return y;
    }
    @SuppressWarnings("unused")
    private static int getBalance(AVLNode n) {
        return (n == null) ? 0 : height(n.left) - height(n.right);
    }

    private static AVLNode deserializeAVLPreorder(String[] tokens, int[] idx) {
        if (tokens[idx[0]].equals("X")) {
            idx[0]++; 
            return null;
        }
        if (!tokens[idx[0]].equals("N"))
            throw new IllegalStateException(
                "Expected N at token "+idx[0]+", got "+tokens[idx[0]]);
        idx[0]++; 
        int data = Integer.parseUnsignedInt(tokens[idx[0]++]);
        AVLNode root = new AVLNode(data);

        root.triNode = deserializeTriePreorder(tokens, idx);

        root.left  = deserializeAVLPreorder(tokens, idx);
        root.right = deserializeAVLPreorder(tokens, idx);

        root.height = 1 + max(height(root.left), height(root.right));
        return root;
    }

    private static TrieNode deserializeTriePreorder(String[] tokens, int[] idx) {
        if (tokens[idx[0]].equals("X")) {
            idx[0]++; 
            return null;
        }
        if (!tokens[idx[0]].equals("T"))
            throw new IllegalStateException(
                "Expected T at token "+idx[0]+", got "+tokens[idx[0]]);
        idx[0]++; 
        boolean isEnd = tokens[idx[0]++].equals("1");
        TrieNode t = new TrieNode();
        t.isEnd = isEnd;
        t.avlRoot = deserializeAVLPreorder(tokens, idx);
        return t;
    }

    private static String readFile(String filename) {
        try {
            byte[] all = Files.readAllBytes(new File(filename).toPath());
            return new String(all, StandardCharsets.UTF_8);
        } catch (IOException e) {
            System.err.println("Error reading "+filename+": "+e);
            return null;
        }
    }

    private static String[] tokenize(String s) {
        return s.trim().split("\\s+");
    }

    private static String englishToDevanagari(String input) {
        StringBuilder out = new StringBuilder();
        int pos = 0, len = input.length();
        while (pos < len) {
            boolean matched = false;
            for (int l = 4; l >= 1; l--) {
                if (pos + l > len) continue;
                String sub = input.substring(pos, pos + l);
                for (Mapping m : mappings) {
                    if (m.eng.equals(sub)) {
                        for (int cp : m.codes) {
                            out.appendCodePoint(cp);
                        }
                        pos += l;
                        matched = true;
                        break;
                    }
                }
                if (matched) break;
            }
            if (!matched) {
                out.append(input.charAt(pos++));
            }
        }
        return out.toString();
    }

    public static void searchPrefix(TrieNode root, String prefix) {
        int[] cps = prefix.codePoints().toArray();
        TrieNode curr = root;
        for (int cp : cps) {
            AVLNode node = curr.avlRoot, found = null;
            while (node != null) {
                if (cp < node.data)      node = node.left;
                else if (cp > node.data) node = node.right;
                else { found = node; break; }
            }
            if (found == null || found.triNode == null) {
                System.out.println("No suggestions");
                return;
            }
            curr = found.triNode;
        }
        System.out.println("Suggestions:");
        List<Integer> buffer = new ArrayList<>();
        for (int cp : cps) buffer.add(cp);
        dfsTrie(curr, buffer);
    }

    private static void dfsTrie(TrieNode t, List<Integer> buffer) {
        if (t.isEnd) {
            buffer.forEach(cp -> 
                System.out.print(new String(Character.toChars(cp)))
            );
            System.out.println();
        }
        traverseAVL(t.avlRoot, buffer);
    }

    private static void traverseAVL(AVLNode n, List<Integer> buffer) {
        if (n == null) return;
        traverseAVL(n.left, buffer);
        buffer.add(n.data);
        dfsTrie(n.triNode, buffer);
        buffer.remove(buffer.size() - 1);
        traverseAVL(n.right, buffer);
    }

    public static void main(String[] args) {
        String pre = readFile("preorder.txt");
        if (pre == null) return;

        String[] tokens = tokenize(pre);
        int[] idx = { 0 };
        TrieNode root = deserializeTriePreorder(tokens, idx);

        System.out.println("Trie reconstructed.");

        Scanner in = new Scanner(System.in, "UTF-8");
        while (true) {
            System.out.print("\nprefix: ");
            if (!in.hasNextLine()) break;
            String eng = in.nextLine().trim();
            if (eng.isEmpty()) break;

            String dev = englishToDevanagari(eng);
            searchPrefix(root, dev);
        }
        in.close();
    }
}
