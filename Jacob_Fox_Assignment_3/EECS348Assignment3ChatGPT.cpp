/*
 * EECS 348 Assignment 3
 * Email Priority Queue Using a MaxHeap
 *
 * Description:
 *     This program prioritizes a CEO's emails using a custom MaxHeap.
 *     Emails are prioritized first by sender category and then by date.
 *     The highest-priority email can be viewed with NEXT and removed
 *     with READ.
 *
 * Input:
 *     EMAIL <sender category>,<subject line>,<date>
 *     NEXT
 *     READ
 *     COUNT
 *
 * Output:
 *     NEXT displays the highest-priority unread email.
 *     COUNT displays the number of unread emails.
 *
 * Collaborators:
 *     None.
 *
 * Other Sources:
 *     ChatGPT was used to generate and analyze an initial implementation.
 *     The final implementation was reviewed and modified for this assignment.
 *
 * Author:
 *     Jacob Fox
 *
 * Creation Date:
 *     September 30, 2026
 *
 * Revision Date:
 *     September 30, 2026
 *
 * Revisions:
 *     Initial C++ implementation using a custom list-based MaxHeap.
 */

#include <iostream>     // Provides input and output functionality.
#include <string>       // Provides the string class.
#include <vector>       // Provides the vector class used as the heap's list.
#include <sstream>      // Provides string stream functionality for parsing input.

using namespace std;


/*
 * Email
 *
 * Stores all information associated with one email.
 * The priority value and date value are stored with the email so
 * that the MaxHeap can compare emails efficiently.
 */
class Email
{
private:
    string sender;       // Stores the sender category.
    string subject;      // Stores the email subject.
    string date;         // Stores the original date string.
    int priority;       // Stores the sender category priority.
    int dateValue;      // Stores the date as YYYYMMDD for comparison.
    long long sequence;  // Stores insertion order to provide a deterministic tie-breaker.

public:

    /*
     * Constructor:
     * Creates an Email object and calculates its priority information.
     */
    Email(string senderCategory, string emailSubject, string emailDate,
          long long emailSequence)
    {
        sender = senderCategory;             // Store the sender category.
        subject = emailSubject;              // Store the subject.
        date = emailDate;                    // Store the original date.
        sequence = emailSequence;            // Store the insertion order.

        priority = calculatePriority();      // Determine sender priority.
        dateValue = calculateDateValue();    // Convert the date for comparison.
    }


    /*
     * Determines the priority associated with the sender category.
     *
     * Boss            = 5
     * Subordinate     = 4
     * Peer             = 3
     * ImportantPerson  = 2
     * OtherPerson     = 1
     */
    int calculatePriority()
    {
        if (sender == "Boss")
        {
            return 5;
        }

        if (sender == "Subordinate")
        {
            return 4;
        }

        if (sender == "Peer")
        {
            return 3;
        }

        if (sender == "ImportantPerson")
        {
            return 2;
        }

        return 1;   // The only remaining valid category is OtherPerson.
    }


    /*
     * Converts MM-DD-YYYY into YYYYMMDD.
     *
     * This allows dates to be compared numerically while preserving
     * chronological ordering.
     */
    int calculateDateValue()
    {
        int month = stoi(date.substr(0, 2));      // Extract the month.
        int day = stoi(date.substr(3, 2));        // Extract the day.
        int year = stoi(date.substr(6, 4));       // Extract the year.

        return year * 10000 + month * 100 + day;  // Return YYYYMMDD.
    }


    /*
     * Returns true when this email should appear before "other"
     * in the MaxHeap.
     */
    bool hasHigherPriority(const Email& other) const
    {
        // Sender category has the highest importance.
        if (priority != other.priority)
        {
            return priority > other.priority;
        }

        // If sender categories match, the newest email comes first.
        if (dateValue != other.dateValue)
        {
            return dateValue > other.dateValue;
        }

        // If everything else is equal, use insertion order to keep
        // the behavior deterministic.
        return sequence > other.sequence;
    }


    /*
     * Displays the email in the exact format required by the assignment.
     */
    void display() const
    {
        cout << "Sender: " << sender << endl;
        cout << "Subject: " << subject << endl;
        cout << "Date: " << date << endl;
    }
};


/*
 * MaxHeap
 *
 * Implements a MaxHeap using a vector as the underlying list.
 *
 * The largest/highest-priority Email is always stored at index 0.
 *
 * Parent index:
 *     (index - 1) / 2
 *
 * Left child:
 *     2 * index + 1
 *
 * Right child:
 *     2 * index + 2
 */
class MaxHeap
{
private:
    vector<Email> heap;     // List-based storage for the MaxHeap.


    /*
     * Moves an element upward until the MaxHeap property is restored.
     *
     * This is used after inserting a new email.
     */
    void heapifyUp(int index)
    {
        // Continue while the current element has a parent.
        while (index > 0)
        {
            int parent = (index - 1) / 2;   // Calculate parent index.

            // Stop if the parent already has higher priority.
            if (!heap[index].hasHigherPriority(heap[parent]))
            {
                break;
            }

            // Swap the child with its parent.
            swap(heap[index], heap[parent]);

            // Continue checking from the parent's old position.
            index = parent;
        }
    }


    /*
     * Moves an element downward until the MaxHeap property is restored.
     *
     * This is used after removing the highest-priority email.
     */
    void heapifyDown(int index)
    {
        int size = static_cast<int>(heap.size());

        while (true)
        {
            int left = 2 * index + 1;     // Calculate left child index.
            int right = 2 * index + 2;    // Calculate right child index.
            int largest = index;          // Assume current node is largest.


            // Check whether the left child has higher priority.
            if (left < size &&
                heap[left].hasHigherPriority(heap[largest]))
            {
                largest = left;
            }


            // Check whether the right child has higher priority.
            if (right < size &&
                heap[right].hasHigherPriority(heap[largest]))
            {
                largest = right;
            }


            // The heap property is already correct.
            if (largest == index)
            {
                break;
            }


            // Move the higher-priority child upward.
            swap(heap[index], heap[largest]);

            // Continue from the child's old position.
            index = largest;
        }
    }


public:

    /*
     * Adds an email to the MaxHeap.
     *
     * Time complexity: O(log n)
     */
    void insert(const Email& email)
    {
        // Place the new email at the end of the list.
        heap.push_back(email);

        // Restore the MaxHeap property.
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }


    /*
     * Returns whether the heap contains no emails.
     *
     * Time complexity: O(1)
     */
    bool empty() const
    {
        return heap.empty();
    }


    /*
     * Returns the number of unread emails.
     *
     * Time complexity: O(1)
     */
    int size() const
    {
        return static_cast<int>(heap.size());
    }


    /*
     * Returns the highest-priority email without removing it.
     *
     * This is what NEXT needs because two NEXT commands in a row
     * must display the same email.
     *
     * Time complexity: O(1)
     */
    const Email& top() const
    {
        return heap[0];
    }


    /*
     * Removes the highest-priority email from the heap.
     *
     * Time complexity: O(log n)
     */
    void removeTop()
    {
        // Do nothing if there are no emails.
        if (heap.empty())
        {
            return;
        }


        // If only one email exists, simply remove it.
        if (heap.size() == 1)
        {
            heap.pop_back();
            return;
        }


        // Move the final element into the root position.
        heap[0] = heap.back();

        // Remove the duplicate final element.
        heap.pop_back();

        // Restore the MaxHeap property.
        heapifyDown(0);
    }
};


/*
 * EmailManager
 *
 * Controls the interaction between the input commands and the MaxHeap.
 * Keeping this logic separate from the heap makes the program easier
 * to maintain and test.
 */
class EmailManager
{
private:
    MaxHeap emailQueue;       // Stores all unread emails.
    long long nextSequence;   // Tracks insertion order for tie-breaking.


    /*
     * Creates an Email object from an EMAIL command.
     */
    Email createEmail(const string& line)
    {
        // Remove the "EMAIL " portion from the beginning of the line.
        string emailData = line.substr(6);

        // Find the commas separating the three fields.
        size_t firstComma = emailData.find(',');
        size_t secondComma = emailData.find(',', firstComma + 1);

        // Extract the sender category.
        string sender = emailData.substr(0, firstComma);

        // Extract the subject between the two commas.
        string subject = emailData.substr(
            firstComma + 1,
            secondComma - firstComma - 1
        );

        // Extract the date after the second comma.
        string date = emailData.substr(secondComma + 1);

        // Create the Email object with its insertion sequence.
        Email email(sender, subject, date, nextSequence);

        // Increment the sequence for the next email.
        nextSequence++;

        return email;
    }


public:

    /*
     * Constructor:
     * Starts the insertion sequence at zero.
     */
    EmailManager()
    {
        nextSequence = 0;
    }


    /*
     * Processes one input command.
     */
    void processCommand(const string& line)
    {
        // EMAIL commands add a new email to the priority queue.
        if (line.rfind("EMAIL ", 0) == 0)
        {
            Email email = createEmail(line);
            emailQueue.insert(email);
        }


        // NEXT displays the highest-priority email without removing it.
        else if (line == "NEXT")
        {
            if (!emailQueue.empty())
            {
                cout << "Next email:" << endl;
                emailQueue.top().display();
            }
        }


        // READ removes the highest-priority email without displaying it.
        else if (line == "READ")
        {
            emailQueue.removeTop();
        }


        // COUNT displays the current number of unread emails.
        else if (line == "COUNT")
        {
            cout << "There are "
                 << emailQueue.size()
                 << " emails to read."
                 << endl;
        }
    }
};


/*
 * Main program
 *
 * Reads commands until the input file reaches its end.
 */
int main()
{
    EmailManager manager;     // Create the object responsible for email management.
    string line;              // Stores one complete input line at a time.


    // Read and process every command from standard input.
    while (getline(cin, line))
    {
        manager.processCommand(line);
    }


    return 0;                 // Indicate successful program completion.
}