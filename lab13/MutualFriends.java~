/*

  CS61C Summer 2013 Lab13, MapReduce II: Finding Mutual Friends

  STUDENT SKELETON

 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.lang.Math;
import java.util.*;
import java.lang.System;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class MutualFriends {
    public enum VerticesCounted { PASSED };

    // allows you to emit two Longs as a key, useful for outputting
    // a friend pair
    public static class TwoLongWritable implements WritableComparable {
        public long node1; // first person
        public long node2; // second person

        // constructor 
        public TwoLongWritable(long node1, long node2) {
            this.node1 = node1;
            this.node2 = node2;
        }

        // required empty constructor
        public TwoLongWritable(){
            // does nothing
        }

        // Serializes object - needed for WritableComparable
        public void write(DataOutput out) throws IOException {
            out.writeLong(node1);
            out.writeLong(node2);
        }

        // Deserializes object - needed for WritableComparable
        public void readFields(DataInput in) throws IOException {
            node1 = in.readLong();
            node2 = in.readLong(); 
        }

        // compareTo method required to implement WritableComparable
        public int compareTo(Object q) {
            // all we really care about is when both node1 and node2 are equal 
            // in the two objects:
            TwoLongWritable o = (TwoLongWritable) q;
            if (this.node1 == o.node1 && this.node2 == o.node2) {
                return 0;
            } else {
                // Otherwise, do something reasonable, but guarantee that no other
                // case besides the one above returns zero.
                // So, sort only on node1, but if node1s are equal, then sort on 
                // node2
                if (this.node1 == o.node1) {
                    if (this.node2 < o.node2) {
                        return -1;
                    } else {
                        return 1;
                    }
                } else {
                    if (this.node1 < o.node1) {
                        return -1;
                    } else {
                        return 1;
                    }
                }
            }
        }

        // hashCode required to implement WritableComparable
        public int hashCode() {
            return (new Long(node1 + node2)).hashCode();
        }

        public String toString(){
            String output = "(" + ((new Long(node1)).toString()) + ", " + ((new Long(node2)).toString()) + ")";
            return output;
        }
    }


    // Custom value class to allow you to emit an array of longs as a value
    // You cannot use this as a key because it does not implement WritableComparable
    public static class EValue implements Writable {
        public long[] transmitArray; // array we want to transmit
 
        // Actual constructor
        public EValue(long[] transmitArray) {
            this.transmitArray = transmitArray;
        }

        // Required empty constructor
        public EValue(){
            // does nothing
        }

        // Serializes object - needed for Writable
        public void write(DataOutput out) throws IOException {
            // store length of transmitArray and transmitArray if it's given
            int length = 0;

            if (transmitArray != null){
                length = transmitArray.length;
            }

            out.writeInt(length);

            for (int i = 0; i < length; i++){
                out.writeLong(transmitArray[i]);
            }
        }

        // Deserializes object - needed for Writable
        public void readFields(DataInput in) throws IOException {
            int length = in.readInt();
            transmitArray = new long[length];
            
            for(int i = 0; i < length; i++){
                transmitArray[i] = in.readLong();
            }

        }

        // Automagically format output as specified in the lab doc
        public String toString(){
            String part2 = "Friends List: ";
            for (int a = 0; a < transmitArray.length; a++){
                part2 += (new Long(transmitArray[a])).toString() + " ";
            }
            return part2;
        }
    }


    /* The first mapper.
     */
    public static class LoaderMap extends Mapper<LongWritable, LongWritable, 
        LongWritable, LongWritable> {

        @Override
        public void map(LongWritable key, LongWritable value, Context context)
                throws IOException, InterruptedException {

            /* *** YOUR CODE HERE *** */
	context.write(key, value);
	    context.write(value, key);
        }
    }


    /* The first reducer. The output of this reducer should be (person, 
     * friends list) */
    public static class LoaderReduce extends Reducer<LongWritable, LongWritable, 
        LongWritable, EValue> {

        public void reduce(LongWritable key, Iterable<LongWritable> values, 
            Context context) throws IOException, InterruptedException {

            /* *** YOUR CODE HERE *** */
int i = 0, j = 0; 
 
	    Set<Long> inter = new HashSet<Long>();
	    
	    for (EValue value : values)
		{
		    Set<Long> tempSet = new HashSet<Long>();
 
		    for (j = 0; j < value.transmitArray.length; ++j)
			{
			    tempSet.add(value.transmitArray[j]);
			}
 
		    if (i == 0)
			{
			    inter = tempSet;
			}
		    else if (i == 1)
			{
			    inter.retainAll(tempSet);
			}
		    ++i;
		}
	    context.write(key, new EValue(inter.toArray(new Long[0])));
        }
    }


    /* The second mapper. This gets its input from LoaderReduce. Thus, there
     * is a mapper called for every (person, friends list) pair
     */
    public static class MutFriendMap extends Mapper<LongWritable, EValue, 
        TwoLongWritable, EValue> {

        @Override
        public void map(LongWritable key, EValue value, Context context)
                throws IOException, InterruptedException {

            /* ***YOUR CODE HERE*** */
                for (int i = 0; i < value.transmitArray.length; ++i)
		{
		    if (key.get() < value.transmitArray[i])
			{
			    context.write(new TwoLongWritable(key.get(), value.transmitArray[i]), value);
			}
		    else
			{
			    context.write(new TwoLongWritable(value.transmitArray[i], key.get()), value);
			}
		}
        }

    }


    /* The second reducer. The output of this reducer should match the sample
     * output given in the lab doc. Luckily, correctly emitting a (key, value) pair
     * (TwoLongWritable, EValue) will do all of the formatting for you.
     */
    public static class MutFriendReduce extends Reducer<TwoLongWritable, EValue, 
        TwoLongWritable, EValue> {

        public void reduce(TwoLongWritable key, Iterable<EValue> values, 
            Context context) throws IOException, InterruptedException {

            /* ***YOUR CODE HERE*** */

        }
   
    }


    /* Excluding the change to job.setNumReduceTasks(VALUE) for Exercise 2, 
     * you shouldn't need to modify anything below this line. We've made
     * all of the tedious Input/Output type changes and Class name changes
     * for you already, in addition to already chaining our two map/reduce
     * phases together. If you think you need to change something below,
     * please talk to your TA first.
     */
    public static void main(String[] rawArgs) throws Exception {
        GenericOptionsParser parser = new GenericOptionsParser(rawArgs);
        Configuration conf = parser.getConfiguration();
        String[] args = parser.getRemainingArgs();

        // Setting up mapreduce job to load in graph, perform our preprocessing step
        Job job = new Job(conf, "load graph");

        job.setNumReduceTasks(40); // DO NOT CHANGE THIS LINE, the setNumReduceTasks
                                   // you need to change for Exercise 2 setup is
                                   // further below

        job.setJarByClass(MutualFriends.class);

        job.setMapOutputKeyClass(LongWritable.class);
        job.setMapOutputValueClass(LongWritable.class);
        job.setOutputKeyClass(LongWritable.class);
        job.setOutputValueClass(EValue.class);

        job.setMapperClass(LoaderMap.class);
        job.setReducerClass(LoaderReduce.class);

        job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setOutputFormatClass(SequenceFileOutputFormat.class);

        // Input from command-line argument, output to predictable place
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path("intermediate"));

        // Actually starts job, and waits for it to finish
        job.waitForCompletion(true);

        // Now, actually generate and output mutual friends list
        job = new Job(conf, "friendlist");
        job.setJarByClass(MutualFriends.class);

        job.setNumReduceTasks(1); // CHANGE THIS LINE WHEN INDICATED TO DO SO
                                  // IN Exercise 2 Setup

        job.setMapOutputKeyClass(TwoLongWritable.class);
        job.setMapOutputValueClass(EValue.class);
        job.setOutputKeyClass(TwoLongWritable.class);
        job.setOutputValueClass(EValue.class);

        job.setMapperClass(MutFriendMap.class);
        job.setReducerClass(MutFriendReduce.class);

        job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        // chain output from previous stage into actual processing stage
        FileInputFormat.addInputPath(job, new Path("intermediate"));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        job.waitForCompletion(true);
    }
}
