import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


public class MemSim {
	
	public static int numFrames = 0;
	public static Scanner scanner = null;
	public static int offSetMask = (1 << 8) - 1;
	public static int pageNumber = ((1 << 16) - 1) ^ offSetMask;
	public static int virtualAddress =0;
	private static boolean LOGGER;
	
	public static void main(String[] args) {
		try {
			scanner = new Scanner(new File(args[0]));
			numFrames = Integer.parseInt(args[1]);
			
			if(args.length == 3) {
				noReplacement();
			}
			else if(args[2].equals("fifo")) {
				fifoReplacemnt();
			}
			else if(args[2].equals("lru")) {
				lruReplacement();
			}
			else if(args[2].equals("opt")) {
				optReplacement();
			}
			else {
				System.out.println("NOT SUPPORTED");
				System.exit(0);
			}
			
		} catch (FileNotFoundException e) {
			System.out.println("Should not be here");
			e.printStackTrace();
		}

	}
	
	public static void noReplacement() {
		
		while(scanner.hasNext()) {
			virtualAddress = scanner.nextInt();
			int offset = virtualAddress & offSetMask;
			int page = virtualAddress & pageNumber;
			
			checkIfInTLB();
			
		}
		
	}
	
	private static Node checkIfInTLB() {
		if(LOGGER) {
		      printf("===== CHECK TLB =====\n");
		   }
		   
		   if(sizeOfTLB == 0) {
		      return null;
		   }
		   
		   Node *iterator = startTLB;
		   int i;
		   for(i = 0; iterator && i < sizeOfTLB; i++, iterator = (Node *)iterator->next) {
		      PageAndFrameNumber *data = (PageAndFrameNumber *)iterator->data;
		      int thingsPageNum = data->pageNum;
		      
		      if(pageNumber == thingsPageNum) {
		         return iterator;
		      }
		   }
		   return NULL;
		
	}

	public static void fifoReplacemnt() {
		while(scanner.hasNext()) {
			
		}
	}
	
	public static void lruReplacement() {
		while(scanner.hasNext()) {
			
		}
	}
	
	public static void optReplacement() {
		while(scanner.hasNext()) {
			
		}
		
	}

}
